CC = gcc
CFLAGS = -g
FILES = main.c file.c scanner.c


all:
	$(CC) $(CFLAGS) $(FILES)



.PHONY : clean

clean:
	-rm a.out