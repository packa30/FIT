CXX=gcc
CXXFLAGS= -Wall -pedantic -Wextra

all: error.c symtable.c instrlist.c generate.c main.c
	$(CXX) -o generate $(CXXFLAGS) $^

clean:
	rm -f generate
