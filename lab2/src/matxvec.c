#include "matxvec.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum split
{
  Row,
  Column,
  Block
};


double_t _sum_vec(const double_t* const vector, uint32_t n)
{
  double_t res = 0;

  for (uint32_t i = 0; i < n; i++)
    res += vector[i];

  return res;
}

void _mul_col_by_const(const double_t** const matrix, double_t* out, double_t digit, uint32_t rows, uint32_t col_num)
{
  for (uint32_t row = 0; row < rows; row++)
    out[row] += (matrix[row][col_num] * digit);
}

double_t _mul_row_by_col(const double_t* const row, const double_t* const col, uint32_t n)
{
  double_t res = 0;

  for (uint32_t i = 0; i < n; i++)
    res += (row[i] * col[i]);

  return res;
}


void _row_split_mul(const double_t** const matrix, const double_t* const vector, double_t* out, uint32_t rows, uint32_t cols)
{
#pragma omp parallel for
  for (uint32_t row = 0; row < rows; row++)
  {
    out[row] = _mul_row_by_col(matrix[row], vector, cols);
  }
}

void _column_split_mul(const double_t** const matrix, const double_t* const vector, double_t* out, uint32_t rows, uint32_t cols)
{
  double_t** cols_vector = malloc(sizeof(double_t*) * cols);
  for (uint32_t col = 0; col < cols; col++)
  {
    cols_vector[col] = malloc(sizeof(double_t) * rows);
    memset(cols_vector, 0, sizeof(double_t) * rows);
  }

#pragma omp parallel for
  for (uint32_t col = 0; col < cols; col++)
  {
    _mul_col_by_const(matrix, cols_vector[col], vector[col], rows, col);
  }

#pragma omp parallel for
  for (uint32_t row = 0; row < rows; row++)
  {
    for (uint32_t col = 0; col < cols; col++)
    {
      out[row] += cols_vector[col][row];
    }
  }

  for (uint32_t row = 0; row < rows; row++)
  {
    free(cols_vector[row]);
  }
  free(cols_vector);
}

void _block_split_mul(const double_t** const matrix, const double_t* const vector, double_t* out, uint32_t rows, uint32_t cols)
{
  uint32_t rows_half = rows / 2;
  uint32_t cols_half = cols / 2;
  /* 0 ... rows_half - 1 rows_half ... rows - 1 */
  /* 0 ... cols_half - 1 cols_half ... cols - 1 */
  double_t* prev_out = malloc(rows * sizeof(double_t));
  double_t* upper_out = prev_out;
  double_t* lower_out = prev_out + rows_half;
  memset(upper_out, 0, rows * sizeof(double_t));

  for (uint32_t row = 0; row < rows_half; row++)
  {
    upper_out[row] += _mul_row_by_col(matrix[row], vector, cols_half);
    lower_out[row] += _mul_row_by_col(matrix[row] + cols_half, vector + cols_half, cols_half);
  }

  for (uint32_t row = rows_half; row < rows; row++)
  {
    upper_out[row] += _mul_row_by_col(matrix[row], vector, cols_half);
    lower_out[row] += _mul_row_by_col(matrix[row] + cols_half, vector + cols_half, cols_half);
  }

  for (uint32_t row = 0; row < rows; row++)
  {
    out[row] = prev_out[row];
  }

  free(prev_out);
}

void mul_mat_by_vec(enum split method, const double_t** const matrix, const double_t* const vector, double_t* out, uint32_t rows, uint32_t cols)
{
  switch (method)
  {
  case Row:
    _row_split_mul(matrix, vector, out, rows, cols);
    break;
  case Column:
    _column_split_mul(matrix, vector, out, rows, cols);
    break;
  case Block:
    if (rows % 2 == 0)
      _block_split_mul(matrix, vector, out, rows, cols);
    break;
  }
}

