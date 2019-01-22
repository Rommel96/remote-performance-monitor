LIB = -pthread

.PHONY: all
all: cliente server

cliente: src/monitorc.c
	gcc -o monitorc src/monitorc.c $(LIB) -I .

server: src/monitord.c
	gcc -I . -o monitord src/monitord.c src/read.c src/csapp.c src/daemon_mode.c $(LIB) -I .


.PHONY: clean
clean:
	rm -f *.o *.txt monitord monitorc *~
