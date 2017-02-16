all:
	rm Main -rf
	g++ -o Main Main.cpp -lcrypto
	./Main
