CC=gcc
src=$(wildcard *.c)
obj=$(src:.cpp=.o)
CFLAGS=-Wall -Wextra -pedantic -g

ifj: $(obj)
	$(CC) $(CFLAGS) -o $@ $^
test:
	chmod +x test.sh
	./test.sh
clean:
	rm  ifj
