#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <math.h>

static inline void freep(void *p) {
  free(*(void **) p);
}

double_t pi_montecarlo(int nthrow)
{
  double_t pi = 0;
  // TODO: Implement calculation
  return pi;
}

void* routine(void* rank)
{
  uint64_t my_rank = (uint64_t)rank;
  // TODO: Run montecarlo calculation
  return NULL;
}


int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Use following format:\n./%s [nthreads] [ntrials]\n", argv[0]);
    return EXIT_FAILURE;
  }

  uint64_t thread_count = strtol(argv[1], NULL, 10);

  __attribute__((cleanup(freep)))
  pthread_t* thread_handler = malloc(thread_count * sizeof(pthread_t));

  for (uint64_t i = 0; i < thread_count; i++)
  {
    uint8_t err = pthread_create(thread_handler + i, NULL, routine, (void*)i);
    if (err)
      fprintf(stderr, "Error while creating %lu pthread\n", i);
  }

  for (uint64_t i = 0; i < thread_count; i++)
  {
    pthread_join(thread_handler[i], NULL);
  }

  return EXIT_SUCCESS;
}
