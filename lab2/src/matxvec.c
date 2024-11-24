#include "matxvec.h"

#include <math.h>
#include <stdint.h>

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

void _mul_col_by_const(const double_t* const col, double_t* out, double_t digit, uint32_t n)
{
  for (uint32_t i = 0; i < n; i++)
    out[i] = (col[i] * digit);
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

void _column_split_mul(const double_t** const matrix, const double_t* const vector, double_t* out, uint32_t n, uint32_t m)
{

}

void _block_split_mul(const double_t** const matrix, const double_t* const vector, double_t* out, uint32_t n, uint32_t m)
{

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

