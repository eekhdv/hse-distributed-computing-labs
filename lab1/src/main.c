#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>
#include <sys/types.h>

#ifdef MONTE_CARLO
#include "monte-carlo.h"
#endif /* ifdef MONTE_CARLO */


static inline void freep(void *p) {
  free(*(void **) p);
}

#ifdef MONTE_CARLO
int monte_carlo(int argc, char *argv[])
{
  static point_t* pointers_arr;
  uint64_t thread_count, throw_count;
  uint64_t circle_points = 0;

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

  for (uint64_t i = 0; i < thread_count; i++)
  {
    pthread_args_t* args = (pthread_args_t*)malloc(sizeof(pthread_args_t));
    args->tid        = i;
    args->arr_size   = i == (thread_count - 1) ? last_thread_arr_size : single_thread_arr_size;
    args->points_arr = pointers_arr + (i * single_thread_arr_size);

    uint8_t err = pthread_create(thread_handler + i, NULL, routine, (void*)args);
    if (err)
      fprintf(stderr, "Error while creating %lu pthread\n", i);
  }

  for (uint64_t i = 0; i < thread_count; i++)
  {
    uint64_t* res;

    pthread_join(thread_handler[i], (void**)&res);
    circle_points += ((uint64_t)*res);

    free(res);
  }

  printf("pi = %lf", (4.0 * (double_t)circle_points) / throw_count);

  free(pointers_arr);
  return EXIT_SUCCESS;
}
#endif

int main(int argc, char *argv[])
{
#ifdef MONTE_CARLO
  if (monte_carlo(argc, argv) != 0) return EXIT_FAILURE;
#elif MANDELBROT
  printf("MANDELBROT Hello world!\n");
#elif RWLOCK
  printf("RWLOCK Hello world!\n");
#else
  printf("Hello world!\n");
#endif

  return EXIT_SUCCESS;
}
