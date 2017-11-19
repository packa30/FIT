CXX=g++
CXXFLAGS=-O2 -g -Wall -pedantic -std=c++11 -pthread -lcrypto -lssl
LDFLAGS=-Wl,-rpath=/usr/local/lib/gcc49/

all: arguments.cpp connection.cpp popcl.cpp
	$(CXX) -o popcl $(CXXFLAGS) $^

clean:
	rm -f popcl
