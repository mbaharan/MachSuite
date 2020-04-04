#include "gemm.h"

void gemm(TYPE m1[N], TYPE m2[N], TYPE prod[N])
{
    int i, j, k;
    TYPE mult;

    TYPE _m1[row_size][col_size];
#pragma HLS array_partition variable = _m1 dim = 2 complete
    TYPE _m2[row_size][col_size];
#pragma HLS array_partition variable = _m2 dim = 1 complete
    TYPE _prod[row_size][col_size];
#pragma HLS array_partition variable = _prod dim = 2 complete

bulk_read:
    for (int rr = 0; rr < row_size; rr++)
    {
        for (int cc = 0; cc < col_size; cc++)
        {
            _m1[rr][cc] = m1[rr * col_size + cc];
            _m2[rr][cc] = m2[rr * col_size + cc];
        }
    }

outer:
    for (i = 0; i < row_size; i++)
    {
    middle:
        for (j = 0; j < col_size; j++)
        {
#pragma HLS UNROLL
            TYPE sum = 0;
        inner:
            for (k = 0; k < row_size; k++)
            {
#pragma HLS PIPELINE
                mult = _m1[i][k] * _m2[k][j];
                sum += mult;
            }
            _prod[i][j] = sum;
        }
    }

bulk_write:
    for (int row = 0; row < row_size; row++)
    {
        for (int col = 0; col < col_size; col++)
        {
            prod[row * col_size + col] = _prod[row][col];
        }
    }
}