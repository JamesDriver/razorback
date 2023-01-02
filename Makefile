all: implant server

server: server.c libs/shell.o libs/func.o libs/net.o

implant: implant.c libs/net.o

clean:
	rm -f *.o implant server libs/*.o