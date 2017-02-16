#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys,socket
#sys.setdefaultencoding('utf-8')
socket.setdefaulttimeout(3)

connect_res = socket.socket()
response = connect_res.connect_ex(('yimiaozhaopin.com', 3340))
if response == 0:
    connect_res.send('testing')
    banner = connect_res.recv(1024)
    print 'banner is ' + banner

connect_res.close()


