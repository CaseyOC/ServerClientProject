all:obj bin c_tcpclient c_tcpserver c_udpclient c_udpserver cpp_tcpserver cpp_tcpclient cpp_udpserver cpp_udpclient

c_tcpclient: obj obj/c_tcpclient.o
	gcc -o bin/c_tcpclient obj/c_tcpclient.o

obj/c_tcpclient.o: src/c_tcpclient.c
	gcc -c src/c_tcpclient.c -o obj/c_tcpclient.o

c_tcpserver: obj obj/c_tcpserver.o
	gcc -o bin/c_tcpserver obj/c_tcpserver.o

obj/c_tcpserver.o: src/c_tcpserver.c
	gcc -c src/c_tcpserver.c -o obj/c_tcpserver.o

c_udpclient: obj obj/c_udpclient.o
	gcc -o bin/c_udpclient obj/c_udpclient.o

obj/c_udpclient.o: src/c_udpclient.c
	gcc -c src/c_udpclient.c -o obj/c_udpclient.o

c_udpserver: obj obj/c_udpserver.o
	gcc -o bin/c_udpserver obj/c_udpserver.o

obj/c_udpserver.o: src/c_udpserver.c
	gcc -c src/c_udpserver.c -o obj/c_udpserver.o

cpp_tcpserver: obj/cpp_tcpserver.o
	g++ -std=c++11 -o bin/cpp_tcpserver obj/cpp_tcpserver.o

obj/cpp_tcpserver.o: src/cpp_tcpserver.cpp
	g++ -c -std=c++11 src/cpp_tcpserver.cpp -o obj/cpp_tcpserver.o

cpp_tcpclient: obj/cpp_tcpclient.o
	g++ -std=c++11 -o bin/cpp_tcpclient obj/cpp_tcpclient.o

obj/cpp_tcpclient.o: src/cpp_tcpclient.cpp
	g++ -c -std=c++11 src/cpp_tcpclient.cpp -o obj/cpp_tcpclient.o

cpp_udpserver: obj/cpp_udpserver.o
	g++ -std=c++11 -o bin/cpp_udpserver obj/cpp_udpserver.o

obj/cpp_udpserver.o: src/cpp_udpserver.cpp
	g++ -c -std=c++11 src/cpp_udpserver.cpp -o obj/cpp_udpserver.o

cpp_udpclient: obj/cpp_udpclient.o
	g++ -std=c++11 -o bin/cpp_udpclient obj/cpp_udpclient.o

obj/cpp_udpclient.o: src/cpp_udpclient.cpp
	g++ -c -std=c++11 src/cpp_udpclient.cpp -o obj/cpp_udpclient.o

obj:
	mkdir obj/

bin:
	mkdir bin/

clean:
	rm -rf obj/*
	rm -rf bin/*
