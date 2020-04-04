#include "gemm.h"

void gemm(TYPE m1[N], TYPE m2[N], TYPE prod[N])
{
    int i, j, k;
    int k_col, i_col;
    TYPE mult;

    TYPE _m1[N];
    TYPE _m2[N];
    TYPE _prod[N];

bulk_read:
    for (int ii = 0; ii < N; ii++)
    {
        _m1[ii] = m1[ii];
        _m2[ii] = m2[ii];
    }

outer:
    for (i = 0; i < row_size; i++)
    {
#pragma HLS PIPELINE II = 1
    middle:
        for (j = 0; j < col_size; j++)
        {
            i_col = i * col_size;
            TYPE sum = 0;
        inner:
            for (k = 0; k < row_size; k++)
            {
                k_col = k * col_size;
                mult = _m1[i_col + k] * _m2[k_col + j];
                sum += mult;
            }
            _prod[i_col + j] = sum;
        }
    }

bulk_write:
    for (int jj = 0; jj < N; jj++)
    {
        prod[jj] = _prod[jj];
    }
}