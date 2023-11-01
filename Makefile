CC = gcc
CFLAGS = -g
FILES = main.c compilation.c


all:
	$(CC) $(CFLAGS) $(FILES)



.PHONY : clean

clean:
	-rm a.out