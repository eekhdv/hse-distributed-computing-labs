#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <semaphore.h>

#include "monte-carlo.h"
#include "timer.h"

static inline void freep(void *p) {
  free(*(void **) p);
}

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

  return (void*)res;
}

int monte_carlo(int argc, char *argv[])
{
  static point_t* pointers_arr;
  uint64_t thread_count, throw_count;
  uint64_t circle_points = 0;
  double start, finish;

  if (argc != 3)
  {
    fprintf(stderr, "Use following format:\n%s [nthreads] [ntrials]\n", argv[0]);
    return EXIT_FAILURE;
  }

  srand(time(NULL));

  thread_count = strtoll(argv[1], NULL, 10);
  throw_count  = strtoll(argv[2], NULL, 10);

  thread_count = thread_count >= (throw_count >> 1) ? 1 : thread_count;

  __attribute__((cleanup(freep)))
  pthread_t* thread_handler = malloc(thread_count * sizeof(pthread_t));

  pointers_arr = (void*)malloc(sizeof(point_t) * throw_count);
  memset((void*)pointers_arr, 0, sizeof(point_t) * throw_count);

  for (uint64_t i = 0; i < throw_count; i++)
  {
    point_t p = {
      .x = get_randpoint(),
      .y = get_randpoint()
    };
    pointers_arr[i] = p;
  }

  uint64_t single_thread_arr_size = throw_count / thread_count;
  uint64_t last_thread_arr_size   = single_thread_arr_size + (thread_count == 1 ? 0 : throw_count % thread_count);

  __attribute__((cleanup(freep)))
  pthread_args_t* thread_args_arr = malloc(thread_count * sizeof(pthread_args_t));

  __attribute__((cleanup(freep)))
  uint64_t** result = malloc(thread_count * sizeof(uint64_t*));

  for (uint64_t i = 0; i < thread_count; i++)
  {
    pthread_args_t* args = thread_args_arr + i;
    args->tid        = i;
    args->arr_size   = i == (thread_count - 1) ? last_thread_arr_size : single_thread_arr_size;
    args->points_arr = pointers_arr + (i * single_thread_arr_size);
  }

  GET_TIME(start);
  for (uint64_t i = 0; i < thread_count; i++)
  {
    uint8_t err = pthread_create(thread_handler + i, NULL, routine, (void*)&thread_args_arr[i]);
    if (err)
      fprintf(stderr, "Error while creating %lu pthread\n", i);
  }

  for (uint64_t i = 0; i < thread_count; i++)
  {
    pthread_join(thread_handler[i], (void**)&result[i]);
  }
  GET_TIME(finish);

  for (uint64_t i = 0; i < thread_count; i++)
  {
    circle_points += *result[i];
    free(result[i]);
  }

  printf("pi = %lf\n", (4.0 * (double_t)circle_points) / throw_count);
  printf("tooks %es [%ld threads, %ld throws]\n", (finish - start), thread_count, throw_count);

  free(pointers_arr);
  return EXIT_SUCCESS;
}
