CC=gcc
STD=c17

CFLAGS=-std=$(STD) -O3

SRC_DIR=src
OUT_DIR=bin

SRC=$(SRC_DIR)/main.c 
OUT=$(OUT_DIR)/main.o


all: $(SRC)
	mkdir -p bin/
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

PHONY: clean

clean:
	rm -f ./$(OUT_DIR)/*.o
