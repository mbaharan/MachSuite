#include "stencil.h"

void stencil(TYPE orig[row_size * col_size], TYPE sol[row_size * col_size], TYPE filter[f_size])
{
    int r, c, k1, k2;
    TYPE temp, mul;

    TYPE _orig[row_size * col_size];
    TYPE _sol[row_size * col_size];
    TYPE _filter[f_size];

bulk_read:
    for (int ii = 0; ii < (row_size * col_size); ii++)
    {
        _orig[ii] = orig[ii];
    }
    for (int jj = 0; jj < f_size; jj++)
    {
        _filter[jj] = filter[jj];
    }

stencil_label1:
    for (r = 0; r < row_size - 2; r++)
    {
    stencil_label2:
        for (c = 0; c < col_size - 2; c++)
        {
            temp = (TYPE)0;
        stencil_label3:
            for (k1 = 0; k1 < 3; k1++)
            {
            stencil_label4:
                for (k2 = 0; k2 < 3; k2++)
                {
#pragma HLS PIPELINE II = 1
                    mul = _filter[k1 * 3 + k2] * _orig[(r + k1) * col_size + c + k2];
                    temp += mul;
                }
            }
            _sol[(r * col_size) + c] = temp;
        }
    }

bulk_write_r:
    for (int rr = 0; rr < row_size - 2; rr++)
    {
    stencil_label_c:
        for (int cc = 0; cc < col_size - 2; cc++)
        {
            sol[(rr * col_size) + cc] = _sol[(rr * col_size) + cc];
        }
    }
}
