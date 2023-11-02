CC = gcc
CFLAGS = -g
FILES = main.c File.c


all:
	$(CC) $(CFLAGS) $(FILES)



.PHONY : clean

clean:
	-rm a.out