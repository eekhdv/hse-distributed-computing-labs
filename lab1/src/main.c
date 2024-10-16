#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <sys/types.h>


#if defined(MONTE_CARLO)
#include "monte-carlo.h"
int (*func)(int, char*[]) = &monte_carlo;
#elif defined(MANDELBROT)
#include "mandelbrot.h"
int (*func)(int, char*[]) = &mandelbrot;
#else
int (*func)(int, char*[]) = NULL;
#endif /* ifdef MANDELBROT */


int main(int argc, char *argv[])
{
  if (func) {
    if ((*func)(argc, argv) != 0) return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
