MODULES := $(wildcard modules/*.c)
MODULES := $(subst .c,.so,$(MODULES))
# OBJFILES := $(subst $(SRCDIR),$(OBJDIR),$(SRCFILES:%.c=%.o))

all: server $(MODULES)

modules/%.so: modules/%.c
	$(CC) $< $(CFLAGS) -shared -fPIC -ldl -o $@

server: server.c libs/modload.o libs/shell.o

implant.o: implant.c # libs/net.o

tmpimplant.o: implant.o modules/implant.so
	objcopy --add-section .mod1=modules/implant.so --set-section-flags .mod1=noload,readonly implant.o tmpimplant.o 

implant: tmpimplant.o
	$(CC) tmpimplant.o -o implant

test:
	gcc implant.c -c -o test.o
	echo "this is my special datathis is my special data" >mydata
	objcopy --add-section .mydata=mydata --set-section-flags .mydata=noload,readonly test.o test2.o
	gcc test2.o -o test

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