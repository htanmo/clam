CC = gcc
EXE = clam
FLAGS = -o3
BIN = bin
SRC = src
REMOVE = rm -f

all: clam clean

clam: main.o shell.o loop.o
	$(CC) $(FLAGS) $(BIN)/*.o -o $(EXE)

main.o: $(SRC)/main.c
	$(CC) -c $(SRC)/main.c -o $(BIN)/main.o

shell.o: $(SRC)/shell.c
	$(CC) -c $(SRC)/shell.c -o $(BIN)/shell.o

loop.o: $(SRC)/loop.c
	$(CC) -c $(SRC)/loop.c -o $(BIN)/loop.o

.PHONY: clean

clean:
	$(REMOVE) $(BIN)/*.o
