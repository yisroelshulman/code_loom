# variables
CC = gcc
CFLAGS = -g -Wall
BIN = bin
SRC = src
OBJ = obj
FILES = src/main.c src/file.c src/scanner.c src/translator.c src/io.c src/crx.c src/result.c src/comparator.c
OBJECTS = obj/main.o obj/file.o obj/crx.o obj/scanner.o obj/translator.o obj/io.o obj/result.o obj/comparator.o


all:
	$(CC) $(CFLAGS) $(FILES)

# creates the .o file for the file.c file, if the obj directory is not there it creates it
obj/file.o: src/file.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# creates the .o file for the scanner.c file, if the obj directory is not there it creates it
obj/scanner.o: src/scanner.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@


dir: $(OBJ) $(BIN)

# obj directory
$(OBJ):
	mkdir $@

# bin directory
$(BIN):
	mkdir $@

# non file/directory recipies
.PHONY: clean all

clean:
	-rm -f a.out
	-rm -f *.class
	-rm -f error.txt
	-rm -f temp.txt
#-rm -r $(OBJ) $(BIN)