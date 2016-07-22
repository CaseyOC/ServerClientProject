all: obj bin c_client c_server

c_client: obj obj/c_client.o
	gcc -o bin/c_client obj/c_client.o

obj/c_client.o: src/c_client.c
	gcc -c src/c_client.c -o obj/c_client.o

c_server: obj obj/c_server.o
	gcc -o bin/c_server obj/c_server.o

obj/c_server.o: src/c_server.c
	gcc -c src/c_server.c -o obj/c_server.o

obj:
	mkdir obj/

bin:
	mkdir bin/

clean:
	rm -rf obj/*
	rm -rf bin/*
