/*
Implementation based on algorithm described in:
"Stencil computation optimization and auto-tuning on state-of-the-art multicore architectures"
K. Datta, M. Murphy, V. Volkov, S. Williams, J. Carter, L. Oliker, D. Patterson, J. Shalf, K. Yelick
SC 2008
*/

#include "stencil.h"

void stencil3d(TYPE C[2], TYPE orig[SIZE], TYPE sol[SIZE])
{
    int i, j, k;
    TYPE sum0, sum1, mul0, mul1;

    TYPE _C[2];
    TYPE _orig[SIZE];
    TYPE _sol[SIZE];

bulk_read:
    for (int jj = 0; jj < SIZE; jj++)
    {
        _orig[jj] = orig[jj];
    }
bulk_read_C:
    for (int hh = 0; hh < 2; hh++)
    {
        _C[hh] = C[hh];
    }

// Handle boundary conditions by filling with original values
height_bound_col:
    for (j = 0; j < col_size; j++)
    {
    height_bound_row:
        for (k = 0; k < row_size; k++)
        {
            _sol[INDX(row_size, col_size, k, j, 0)] = _orig[INDX(row_size, col_size, k, j, 0)];
            _sol[INDX(row_size, col_size, k, j, height_size - 1)] = _orig[INDX(row_size, col_size, k, j, height_size - 1)];
        }
    }
col_bound_height:
    for (i = 1; i < height_size - 1; i++)
    {
    col_bound_row:
        for (k = 0; k < row_size; k++)
        {
            _sol[INDX(row_size, col_size, k, 0, i)] = _orig[INDX(row_size, col_size, k, 0, i)];
            _sol[INDX(row_size, col_size, k, col_size - 1, i)] = _orig[INDX(row_size, col_size, k, col_size - 1, i)];
        }
    }
row_bound_height:
    for (i = 1; i < height_size - 1; i++)
    {
    row_bound_col:
        for (j = 1; j < col_size - 1; j++)
        {
            _sol[INDX(row_size, col_size, 0, j, i)] = _orig[INDX(row_size, col_size, 0, j, i)];
            _sol[INDX(row_size, col_size, row_size - 1, j, i)] = _orig[INDX(row_size, col_size, row_size - 1, j, i)];
        }
    }

// Stencil computation
loop_height:
    for (i = 1; i < height_size - 1; i++)
    {
    loop_col:
        for (j = 1; j < col_size - 1; j++)
        {
#pragma HLS PIPELINE II = 1
        loop_row:
            for (k = 1; k < row_size - 1; k++)
            {
                sum0 = _orig[INDX(row_size, col_size, k, j, i)];
                sum1 = _orig[INDX(row_size, col_size, k, j, i + 1)] +
                       _orig[INDX(row_size, col_size, k, j, i - 1)] +
                       _orig[INDX(row_size, col_size, k, j + 1, i)] +
                       _orig[INDX(row_size, col_size, k, j - 1, i)] +
                       _orig[INDX(row_size, col_size, k + 1, j, i)] +
                       _orig[INDX(row_size, col_size, k - 1, j, i)];
                mul0 = sum0 * _C[0];
                mul1 = sum1 * _C[1];
                _sol[INDX(row_size, col_size, k, j, i)] = mul0 + mul1;
            }
        }
    }

bulk_write:
    for (int kk = 0; kk < SIZE; kk++)
    {
        sol[kk] = _sol[kk];
    }
}
