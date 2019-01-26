LIB = -lpthread

.PHONY: all
all: cliente server

cliente: src/cliente.c
	gcc -I . -o monitorc src/cliente.c src/monitor.pb-c.c -L/usr/local/lib -lprotobuf-c $(LIB)

server: src/server.c read.o csapp.o daemon_mode.o
	gcc -I . -o monitord src/server.c read.o csapp.o daemon_mode.o src/monitor.pb-c.c -L/usr/local/lib -lprotobuf-c $(LIB)

read.o: src/read.c 
	gcc -I . -c src/read.c src/csapp.c src/monitor.pb-c.c -L/usr/local/lib -lprotobuf-c $(LIB)

daemon_mode.o: src/daemon_mode.c csapp.o
	gcc -I . -c src/daemon_mode.c src/csapp.c $(LIB)

csapp.o: src/csapp.c
	gcc -I . -c src/csapp.c

.PHONY: clean
clean:
	rm -f *.o *.txt monitord monitorc *~
