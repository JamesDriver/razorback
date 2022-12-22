all: lp middleman

lp: lp.c net.o

middleman: middleman.c net.o

clean:
	rm -f *.o lp middleman