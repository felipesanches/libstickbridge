all: library

install:
	cp stickbridge.h /usr/local/include/
	cp libstickbridge.a /usr/local/lib

example:
	gcc example_spi_bridge.c -lstickbridge -lftdi -o ./example_spi_bridge

library:
	gcc -Wall -c stickbridge.c
	ar -cvq libstickbridge.a stickbridge.o

clean:
	rm -f *.a *.o example_spi_bridge
