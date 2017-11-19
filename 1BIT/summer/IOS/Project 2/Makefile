#      Subor : Makefile
#      Riešenie IOS, projekt c.2
#      Autor: Peter Grofčík, FIT 1BIT

NAME = proj2
EXEC = $(NAME)
sources = $(NAME).c

CC = gcc
RM = rm -f

CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LDFLAGS = -lrt -pthread

OBJFILES = $(sources:.c=.o)

.PHONY : all

all : $(EXEC) 

%.o : %.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $(OBJFILES) $(LDFLAGS)

clean:
	$(RM) *.o core *.out

cleanall: clean
	$(RM) $(EXEC)

zip: $(NAME).c  Makefile
	zip xgrofc00.zip $(NAME).c  Makefile
