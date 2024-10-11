#include "monte-carlo.h"
#include <stdint.h>
#include <stdlib.h>

double_t get_randpoint()
{
  return  (double_t)rand() / (double_t)RAND_MAX;
}

void* routine(void* vargs)
{
  uint64_t circle_points = 0;
  uint64_t* res = (uint64_t*)malloc(sizeof(uint64_t));

  pthread_args_t* args = (pthread_args_t*)vargs;
  uint64_t arr_size = args->arr_size;

  for (uint64_t i = 0; i < arr_size; i++)
  {
    point_t p = args->points_arr[i];
    double_t dist = p.x * p.x + p.y * p.y;
    if (dist <= 1.0)
      circle_points++;
  }
  *res = circle_points;

  free(vargs);
  return (void*)res;
}

