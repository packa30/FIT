CC = gcc
OPT = -O2
CFLAGS = -std=c99 -Wall -Wextra -pedantic

all: primes steg-decode

steg-decode: steg-decode.c ppm.c ppm.h bit_array.h eratosthenes.h eratosthenes.c error.c error.h
		$(CC) $(OPT) $(CFLAGS) eratosthenes.c steg-decode.c error.c ppm.c -lm -o steg-decode


primes: error.c error.h eratosthenes.c eratosthenes.h bit_array.h primes.c
		$(CC) $(OPT) $(CFLAGS) error.c eratosthenes.c primes.c -lm -o primes
		$(CC) $(OPT) $(CFLAGS) -DUSE_INLINE primes.c eratosthenes.c error.c -lm -o primes-inline

clean:
	rm -f steg-decode primes primes-inline

zip:
	zip xgrofc00.zip *.c *.h Makefile
