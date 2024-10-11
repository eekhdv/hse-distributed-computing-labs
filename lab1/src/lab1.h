#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <math.h>


typedef struct
{
  double_t x;
  double_t y;
} point_t;

typedef struct
{
  uint64_t tid;

  point_t* points_arr;
  uint64_t arr_size;
} pthread_args_t;


double_t get_randpoint();

void* routine(void* vargs);
