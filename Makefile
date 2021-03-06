all: library

install:
	cp stickbridge.h /usr/local/include/
	cp libstickbridge.a /usr/local/lib

library:
	gcc -Wall -c stickbridge.c
	ar -cvq libstickbridge.a stickbridge.o

clean:
	rm -f *.a *.o
