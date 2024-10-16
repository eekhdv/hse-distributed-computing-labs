#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <complex.h>

#include "mandelbrot.h"
#include "timer.h"

static uint64_t g_npoints;

static uint64_t g_mandelbrot_npoints = 0;
static complex double* g_mandelbrot_points_arr;

pthread_mutex_t g_mutex;

static inline void freep(void *p) {
  free(*(void **) p);
}

int is_mandelbrotset(double complex c)
{
  double complex z = 0;

  for (uint16_t i = 0; i < 4000; i++)
  {
    z = z * z + c;
    if (cabs(z) >= 2.0) return 1;
  }
  return 0;
}

void* routine(void* vargs)
{
  double complex c = 0;
  pthread_args_t* args = (pthread_args_t*)vargs;
  double_t end = args->x_end;

  for (double_t x = args->x_start; x < end; x += 0.00015)
  {
    for (double_t y = -1; y < 1; y += 0.00015)
    {
      c = x + y * I;
      if (is_mandelbrotset(c) == 0)
      {
        pthread_mutex_lock(&g_mutex);
        if (g_mandelbrot_npoints >= g_npoints)
        {
          pthread_mutex_unlock(&g_mutex);
          break;
        }

        g_mandelbrot_points_arr[g_mandelbrot_npoints] = c;
        g_mandelbrot_npoints++;

        pthread_mutex_unlock(&g_mutex);
      }
    }
    if (g_mandelbrot_npoints >= g_npoints) break;
  }
  pthread_exit(NULL);
}


int mandelbrot(int argc, char *argv[])
{
  uint64_t thread_count;
  double start, finish;

  if (argc != 3)
  {
    fprintf(stderr, "Use following format:\n%s [nthreads] [ntrials]\n", argv[0]);
    return EXIT_FAILURE;
  }

  thread_count = strtoll(argv[1], NULL, 10);
  g_npoints  = strtoll(argv[2], NULL, 10);

  double_t x_part_size = (4.0 / (double_t)thread_count);

  __attribute__((cleanup(freep)))
  pthread_t* thread_handler = malloc(thread_count * sizeof(pthread_t));

  g_mandelbrot_points_arr = malloc(sizeof(complex double) * g_npoints);

  pthread_mutex_init(&g_mutex, NULL);

  __attribute__((cleanup(freep)))
  pthread_args_t* thread_args_arr = malloc(thread_count * sizeof(pthread_args_t));
  for (uint64_t i = 0; i < thread_count; i++)
  {
    pthread_args_t* args = thread_args_arr + i;
    args->tid     = i;
    args->x_start = -2.0 + x_part_size * i;
    args->x_end   = args->x_start + x_part_size;
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
    pthread_join(thread_handler[i], NULL);
  }
  GET_TIME(finish);

  FILE* file = fopen("output.csv", "w");
  if (file != NULL)
  {
    for (uint64_t i = 0; i < g_mandelbrot_npoints; i++)
    {
      fprintf(file, "(%lf, %lf)\n", creal(g_mandelbrot_points_arr[i]), cimag(g_mandelbrot_points_arr[i]));
    }
  }
  else
  {
    fprintf(stderr, "Cannot open file %s\n", "output.csv");
  }
  fclose(file);

  printf("See result in the ./output.csv\n");
  printf("tooks %lfs [ %ld threads, %ld points ]\n", (finish - start), thread_count, g_npoints);

  pthread_mutex_destroy(&g_mutex);
  free(g_mandelbrot_points_arr);

  return EXIT_SUCCESS;
}
