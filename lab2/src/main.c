#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>

#include <mpi.h>


int32_t (*func)(int, char*[]) = NULL;


void print_usage()
{
  fprintf(stderr, "Usage:\n");

}


int main(int argc, char *argv[])
{
  int32_t comm_size, rank;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows, cols; /* mat(rows x cols) x vec(cols x 1) */

  if (argc >= 3) {
    rows = atoi(argv[1]);
    cols = atoi(argv[2]);

    if (func) {
      if ((*func)(argc, argv) != 0) return EXIT_FAILURE;
    }
  }
  else {
    if (rank == 0)
      fprintf(stderr, "Usage: mpiexec -n <copies> %s <rows> <cols>\
                    \nRequired arguments:\
                    \n  'copies' - number of copies of the program\
                    \n  'rows'   - number of rows\
                    \n  'cols'   - number of columns\n", argv[0]);
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
