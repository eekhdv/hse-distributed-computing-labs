#pragma once

#include <complex.h>
#include <math.h>
#include <stdint.h>

typedef struct
{
  uint64_t tid;

  double_t x_start;
  double_t x_end;
} pthread_args_t;

int mandelbrot(int argc, char *argv[]);
