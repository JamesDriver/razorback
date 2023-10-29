


all: server modules/test.so #implant

modules/test.so: modules/test.c
	$(CC) modules/test.c $(CFLAGS) -shared -fPIC -ldl -o modules/test.so

server: server.c libs/modload.o libs/shell.o

# implant: implant.c libs/net.o

clean:
	rm -f *.o implant server libs/*.o modules/*.so


# CFLAGS += -O0 -Wall -Wextra -Wpedantic
# CFLAGS += -Wvla -Wwrite-strings -Waggregate-return -Wfloat-equal
# LDFLAGS += -ldl -L. -lldtest

# all: libldtest.a loader modules/test1.so modules/test2.so

# loader: loader.c
# 	$(CC) loader.c $(CFLAGS) -ldl -L. -lldtest -o loader

# libldtest.a: libldtest.a(test.o)

# # I know this is redundant, but I can't be bothered to look up makefile syntax rn
# modules/test1.so: modules/test1.c
# 	$(CC) modules/test1.c $(CFLAGS) -shared -ldl -L. -lldtest -o modules/test1.so

# modules/test2.so: modules/test2.c
# 	$(CC) modules/test2.c $(CFLAGS) -shared -ldl -L. -lldtest -o modules/test2.so

# %.so: %.c
# 	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<

# implant: implant.c libs/net.o

# .PHONY: debug
# debug: CFLAGS += -g3 -O0
# debug: all

# clean:
# 	rm -f *.o implant server loader libs/*.o modules/*.so