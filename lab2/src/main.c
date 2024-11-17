#include <stdio.h>
#include <stdlib.h>

#include <omp.h>
#include <sys/types.h>


int32_t (*func)(int, char*[]) = NULL;


int main(int argc, char *argv[])
{
  if (func) {
    if ((*func)(argc, argv) != 0) return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
