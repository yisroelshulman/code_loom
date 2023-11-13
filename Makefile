CC = gcc
CFLAGS = -g -Wall
BIN = bin
SRC = src
OBJ = obj
FILES = src/main.c src/file.c src/scanner.c src/translator.c src/io.c
OBJECTS = obj/main.o obj/file.o obj/scanner.o obj/translator.o obj/io.o


all: | dir
	$(CC) $(CFLAGS) $(FILES)



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

.PHONY: clean all

clean:
	-rm -r $(OBJ) $(BIN)