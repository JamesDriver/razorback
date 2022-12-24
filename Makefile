all: implant server

server: server.c net.o functions/parser.o functions/func.o

implant: implant.c net.o

clean:
	rm -f *.o implant server functions/*.o