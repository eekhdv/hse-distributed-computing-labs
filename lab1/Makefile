CC=gcc

ifeq ($(TARGET), RWLOCK)
STD=gnu9x
else
STD=c17
endif

ifeq ($(TARGET), RWLOCK)
FLAGS=
endif

ifeq ($(TARGET), MONTE_CARLO)
FLAGS=-DMONTE_CARLO
endif

ifeq ($(TARGET), MANDELBROT)
FLAGS=-DMANDELBROT
endif

CFLAGS=-std=$(STD) $(FLAGS) -O3 -Werror -Wall -lm -pthread

INC_DIR=inc
SRC_DIR=src
OUT_DIR=bin

INC=\
-I$(INC_DIR)/

ifeq ($(TARGET), MONTE_CARLO)
SRC=$(SRC_DIR)/main.c $(SRC_DIR)/monte-carlo.c
OUT=$(OUT_DIR)/monte-carlo.o
endif

ifeq ($(TARGET), MANDELBROT)
SRC=$(SRC_DIR)/main.c $(SRC_DIR)/mandelbrot.c
OUT=$(OUT_DIR)/mandelbrot.o
endif

ifeq ($(TARGET), RWLOCK)
SRC=$(SRC_DIR)/pth_ll_rwl/rwlock.c $(SRC_DIR)/pth_ll_rwl/my_rand.c $(SRC_DIR)/pth_ll_rwl/pth_ll_rwl.c
OUT=$(OUT_DIR)/rwlock.o
endif

all: $(SRC)
	mkdir -p bin/
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $(OUT)

PHONY: clean

clean:
	rm -f ./$(OUT_DIR)/*.o
