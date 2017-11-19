
#      Subor : Makefile
#      Riešenie IJC-DU2, priklad 1-2
#      Autor: Peter Grofčík, FIT 1BIT


CC=gcc
CXX=g++
CFLAGS=-std=c99 -O2 -g -Wall -Wextra -pedantic
CXXFLAGS=-std=c++11 -O2 -g -Wall -pedantic

LIB=hash_function.o htab_init.o htab_lookup_add.o htab_foreach.o htab_remove.o htab_clear.o htab_free.o

all: tail tail2 wordcount wordcount-dynamic

tail: tail.c
	$(CC) $(CFLAGS) tail.c -o tail

tail2: tail2.cc
	$(CXX) $(CXXFLAGS) tail2.cc -o tail2

wordcount: wordcount.o io.o io.h libhtable.a 
	$(CC) $(CFLAGS) wordcount.o io.o libhtable.a -o wordcount

wordcount-dynamic: wordcount.o io.o io.h libhtable.so
	$(CC) $(CLAFGS) -Wl,-rpath,. wordcount.o io.o -o wordcount-dynamic -L. -lhtable

libhtable.a: $(LIB) htable.h hash_function.h
	ar crs libhtable.a $(LIB)

libhtable.so: $(LIB) htable.h hash_function.h
	$(CC) $(CFLAGS) -shared -fPIC $(LIB) -o libhtable.so
	
%.o : %.c 
	$(CC) $(CFLAGS) -fPIC -c $<

clean:
	rm -f tail tail2 wordcount-dynamic wordcount *.o libhtable.so libhtable.a

zip: *.c *.cc *.h Makefile
	zip xgrofc00.zip *.c *.cc *.h Makefile
