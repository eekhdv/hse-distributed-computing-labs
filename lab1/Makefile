CC=gcc
STD=c17

CFLAGS=-std=$(STD) -O3 -Werror -Wall

SRC_DIR=src
OUT_DIR=bin

SRC_MONTE_CARLO=$(SRC_DIR)/main.c $(SRC_DIR)/monte-carlo.c
SRC_MANDELBROT=$(SRC_DIR)/main.c
SRC_RWLOCK=$(SRC_DIR)/main.c

OUT_MONTE_CARLO=$(OUT_DIR)/monte-carlo.o
OUT_MANDELBROT=$(OUT_DIR)/mandelbrot.o
OUT_RWLOCK=$(OUT_DIR)/rwlock.o


all: $(SRC)
	mkdir -p bin/
	$(CC) $(CFLAGS) -DMONTE_CARLO $(SRC_MONTE_CARLO) -o $(OUT_MONTE_CARLO)
	$(CC) $(CFLAGS) -DMANDELBROT  $(SRC_MANDELBROT) -o $(OUT_MANDELBROT)
	$(CC) $(CFLAGS) -DRWLOCK      $(SRC_RWLOCK) -o $(OUT_RWLOCK)

PHONY: clean

clean:
	rm -f ./$(OUT_DIR)/*.o
