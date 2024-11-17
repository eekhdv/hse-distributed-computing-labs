CC=gcc

FLAGS=
STD=c23

CFLAGS=-std=$(STD) $(FLAGS) -O3 -Werror -Wall -lm -pthread

INC_DIR=inc
SRC_DIR=src
OUT_DIR=bin

INC=-I$(INC_DIR)/

SRC=$(SRC_DIR)/main.c
OUT=$(OUT_DIR)/bin.o

all: $(SRC)
	mkdir -p bin/
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $(OUT)

PHONY: clean

clean:
	rm -f ./$(OUT_DIR)/*.o
