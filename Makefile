all: implant server

server: server.c net.o

implant: implant.c net.o

clean:
	rm -f *.o implant server