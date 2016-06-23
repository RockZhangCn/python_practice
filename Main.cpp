#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <errno.h>

const int BUFFER_SIZE = 1024;

int main()
{
    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);

    if(sock_cli < 0)//-1
    {
        return errno;
    }

    int block_flags = 0, nonblock_flags = 0;

    if((block_flags = fcntl(sock_cli, F_GETFL, 0)) < 0)//-1
    {
        close(sock_cli);
        return errno;
    }

    if((nonblock_flags = fcntl(sock_cli, F_SETFL, nonblock_flags | O_NONBLOCK )) < 0)
    {
        close(sock_cli);
        return errno;
    }

    ///定义sockaddr_in
    const char * dnsServerIP = "119.29.29.29";
    //const char * dnsServerIP = "10.29.29.29";
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(80);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr(dnsServerIP);  ///服务器ip
    
    ///连接服务器，成功返回0，错误返回-1
    if (int conRet = connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) //-1
    {
        if(errno != EINPROGRESS)
        {
            close(sock_cli);
            return errno;
        }
    }
    else //OK 
    {

        printf("Connected\n");
    }


    fd_set fdr, fdw;

    FD_ZERO(&fdr);
    FD_ZERO(&fdw);

    FD_SET(sock_cli, &fdr);
    FD_SET(sock_cli, &fdw);

    struct timeval timeout={3,0};
    int res = select(sock_cli + 1, &fdr, &fdw, NULL, &timeout);
    if(res < 0)//-1  error occured.
    {
        close(sock_cli);
        return errno;
    }

    if(res == 0)//time out man page.
    {
        printf("timeout 0\n");
        close(sock_cli);
        return errno;

    }

    printf("%d\n", inet_addr("192.168.1.1"));
    if(res == 1)
    {
        printf("Connected 2\n");
    }

    if(res == 2)
    {
        printf("timeout 2\n");
        close(sock_cli);
        return errno;
    }

    if((nonblock_flags = fcntl(sock_cli, F_SETFL, block_flags )) < 0)
    {
        close(sock_cli);
        return errno;
    }

    int retsnd = setsockopt(sock_cli, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
    int retrcv = setsockopt(sock_cli, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));


    char sendbuf[BUFFER_SIZE];
    memset(sendbuf, 0, BUFFER_SIZE);
    strcat(sendbuf, "GET /d?dn=qq.com HTTP 1.1\r\n");
    strcat(sendbuf, "Host: 119.29.29.29:80\r\n");
    strcat(sendbuf, "UserAgent: QQBrowser X5\r\n");
    strcat(sendbuf, "Connection: Keep-Alive\r\n");
    strcat(sendbuf, "Accept: text/html,*/*;q=0.8\r\n\r\n");

    char recvbuf[BUFFER_SIZE];
    memset(recvbuf, 0, BUFFER_SIZE);

    int sendlen = send(sock_cli, sendbuf, strlen(sendbuf),0); ///发送
    if(sendlen == -1)
    {
        return errno;
    }

    printf("Here2\n");
    int recvlen = recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收
    if(recvlen != -1)//OK.
    {
        char * split = strstr(recvbuf,"\r\n\r\n");
        char * content = split + 4; 
        if(split != NULL)
        {
            printf("%s\n", content );
            
            char * comma = strstr(content, ";");
            if(comma != NULL)
                *comma = 0;

            printf("%s\n", content );

        }
    }
    else
    {
        printf("Here3\n");
        return errno;
    }

    close(sock_cli);
    return 0;
}
