all: obj bin c_client c_server cpp_server cpp_client

c_client: obj obj/c_client.o
	gcc -o bin/c_client obj/c_client.o

obj/c_client.o: src/c_client.c
	gcc -c src/c_client.c -o obj/c_client.o

c_server: obj obj/c_server.o
	gcc -o bin/c_server obj/c_server.o

obj/c_server.o: src/c_server.c
	gcc -c src/c_server.c -o obj/c_server.o

cpp_server: obj/cpp_server.o
	g++ -std=c++11 -o bin/cpp_server obj/cpp_server.o

obj/cpp_server.o: src/cpp_server.cpp
	g++ -c -std=c++11 src/cpp_server.cpp -o obj/cpp_server.o

cpp_client: obj/cpp_client.o
	g++ -std=c++11 -o bin/cpp_client obj/cpp_client.o

obj/cpp_client.o: src/cpp_client.cpp
	g++ -c -std=c++11 src/cpp_client.cpp -o obj/cpp_client.o

obj:
	mkdir obj/

bin:
	mkdir bin/

clean:
	rm -rf obj/*
	rm -rf bin/*
