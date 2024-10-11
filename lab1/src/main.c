#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>

static inline void freep(void *p) {
  free(*(void **) p);
}

typedef struct
{
  double_t x;
  double_t y;
} point_t;

typedef struct
{
  point_t* points_arr;

  uint64_t tid;
  uint16_t arr_size;
} pthread_args_t;

double_t pi_montecarlo(int nthrow)
{
  double_t pi = 0;
  // TODO: Implement calculation
  return pi;
}

void* routine(void* vargs)
{
  pthread_args_t* args = (pthread_args_t*)vargs;

  // TODO: Run montecarlo calculation
  free(vargs);
  return NULL;
}

double_t get_randpoint()
{
  return (double)rand() / (double)RAND_MAX;
}


int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Use following format:\n./%s [nthreads] [ntrials]\n", argv[0]);
    return EXIT_FAILURE;
  }

  srand(time(NULL));

  static point_t* pointers_arr;

  uint64_t thread_count = strtol(argv[1], NULL, 10);
  uint64_t throw_count  = strtol(argv[2], NULL, 10);

  thread_count = thread_count <= throw_count >> 2 ? 1 : throw_count;

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

  uint16_t single_thread_arr_size = 0; // TODO: 
  uint16_t last_thread_arr_size = single_thread_arr_size + (throw_count % thread_count);

  for (uint64_t i = 0; i < thread_count; i++)
  {
    pthread_args_t* args = (pthread_args_t*)malloc(sizeof(pthread_args_t));
    args->tid        = i;
    args->arr_size   = single_thread_arr_size + (i == (thread_count - 1) ? last_thread_arr_size : 0);
    args->points_arr = pointers_arr + (i * single_thread_arr_size);

    uint8_t err = pthread_create(thread_handler + i, NULL, routine, (void*)args);
    if (err)
      fprintf(stderr, "Error while creating %lu pthread\n", i);
  }

  for (uint64_t i = 0; i < thread_count; i++)
  {
    pthread_join(thread_handler[i], NULL);
  }

  free(pointers_arr);

  return EXIT_SUCCESS;
}
