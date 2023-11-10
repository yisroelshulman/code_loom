CC = gcc
CFLAGS = -g -Wall
FILES = main.c file.c scanner.c translator.c io.c


all:
	$(CC) $(CFLAGS) $(FILES)



.PHONY : clean all

clean:
	-rm a.out