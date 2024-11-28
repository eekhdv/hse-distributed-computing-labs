#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include <mpi.h>
#include <time.h>

#include "matxvec.h"

int32_t comm_size = -1, rank = -1;

void input_vector_row_div(double_t* v, uint32_t n)
{
  if (rank == 0)
  {
    for (uint32_t i = 0 ; i < n ; i++)
    {
      v[i] = (double_t)(rand() % 100) / 7;
    }
  }

  MPI_Bcast(v, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}


void print_usage(char *bin_name)
{
  fprintf(stderr, "Usage: mpiexec -n <copies> %s <rows> <cols>\n\
                \nRequired arguments:\
                \n  'copies' - number of parallel processes\
                \n  'rows'   - number of rows\
                \n  'cols'   - number of columns\n", bin_name);

}

int main(int argc, char *argv[])
{
  double_t* vec = NULL;
  double_t* out_vec = NULL;
  double_t** matrix = NULL;

  uint32_t rows, cols; /* mat(rows x cols) x vec(cols x 1) = res(rows x 1) */

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (argc < 3) {
    if (rank == 0) print_usage(argv[0]);

    goto finalize;
  }

  if (rank == 0)
    srand(time(NULL));

  /************************/
  /*** Data preparation ***/
  /************************/
  rows = atoi(argv[1]);
  cols = atoi(argv[2]);

  if (comm_size != rows) {
    if (rank == 0)
      fprintf(stderr, "[Error] Number of processes and number of rows should be equal\n");
    goto finalize;
  }

  vec = malloc(cols * sizeof(double_t));
  input_vector_row_div(vec, cols);

  matrix = malloc(rows * sizeof(double_t*));
  for (uint32_t row = 0; row < rows; row++) {
    matrix[row] = malloc(cols * sizeof(double_t));
    input_vector_row_div(matrix[row], cols);
  }

  if (rank == 0) {
    out_vec = malloc(rows * sizeof(double_t));
    memset(out_vec, 0, rows * sizeof(double_t));
  }

  if (rank == 0) {
    printf("\nMatrix (%dx%d):\n", rows, cols);
    for (uint32_t row = 0; row < rows; row++) {
      for (uint32_t col = 0; col < cols; col++) {
        printf("%.2lf ", matrix[row][col]);
      }
      putc('\n', stdout);
    }
    printf("Vector (%dx1):\n", cols);
    for (uint32_t col = 0; col < cols; col++) {
      printf("%.2lf ", vec[col]);
    }
    putc('\n', stdout);
  }
  /************************/
  MPI_Barrier(MPI_COMM_WORLD);

  // mul_mat_by_vec(Row, matrix, vec, out_vec, rows, cols, rank, comm_size);
  // mul_mat_by_vec(Column, matrix, vec, out_vec, rows, cols, rank, comm_size);
  mul_mat_by_vec(Block, matrix, vec, out_vec, rows, cols, rank, comm_size);

  if (rank == 0) {
    printf("\nResult vector (%dx1):\n", rows);
    for (uint32_t row = 0; row < rows; row++) {
      printf("%.4lf ", out_vec[row]);
    }
  }

  /**************************/
  /*** Clean and finilize ***/
  /**************************/
  for (uint32_t row = 0; row < rows; row++) {
    free(matrix[row]);
  }
  free(matrix);
  free(vec);
  if (rank == 0)
    free(out_vec);

finalize:
  MPI_Finalize();
  /**************************/

  return EXIT_SUCCESS;
}
