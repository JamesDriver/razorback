CFLAGS += -Wall -Wextra -Wpedantic
CFLAGS += -Wvla -Wwrite-strings -Waggregate-return -Wfloat-equal
LDFLAGS += -ldl -L. -lldtest

all: libldtest.a loader modules/test1.so modules/test2.so

libldtest.a: libldtest.a(test.o)



%.so: %.c
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<

implant: implant.c libs/net.o

.PHONY: debug
debug: CFLAGS += -g3 -O0
debug: all

clean:
	rm -f *.o implant server loader libs/*.o modules/*.so



# server: LDFLAGS += -lncurses
# server: server.c
# 	gcc -o $@ $^ $(LDFLAGS)
#libs/shell.o libs/func.o libs/net.o


# app = dlopen_sample
# lib = libcat.so libdog.so

# CFLAGS = -Wall -ansi -pedantic
# LDFLAGS = -ldl

# all: $(app) $(lib)

# lib%.so: %.c
# 	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<

# clean:
# 	$(RM) $(app) $(lib)

# run: all
# 	./$(app) cat
# 	./$(app) dog
# 	-./$(app) bear

# $(lib): animal.h