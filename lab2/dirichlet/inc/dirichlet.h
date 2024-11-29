#pragma once

#include <math.h>
#include <stdint.h>

void dirichlet_method(double_t** const matrix, double_t* const vector, double_t* out, uint32_t rows, uint32_t cols, int32_t rank, int32_t comm_size);