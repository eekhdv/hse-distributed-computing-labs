#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <sys/types.h>

#ifdef MONTE_CARLO
#include "monte-carlo.h"
#endif /* ifdef MONTE_CARLO */

#ifdef MANDELBROT
#include "mandelbrot.h"
#endif /* ifdef MANDELBROT */

int main(int argc, char *argv[])
{
#if defined(MONTE_CARLO)
  if (monte_carlo(argc, argv) != 0) return EXIT_FAILURE;
#elif defined(MANDELBROT)
  if (mandelbrot(argc, argv) != 0) return EXIT_FAILURE;
#else
  printf("Hello world!\n");
#endif

  return EXIT_SUCCESS;
}
