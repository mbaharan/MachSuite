#include "gemm.h"

void gemm(TYPE m1[N], TYPE m2[N], TYPE prod[N], command_t cmd)
{

#pragma HLS interface m_axi port = m1 offset = direct bundle = read_data_port_m1 num_write_outstanding = 1 max_read_burst_length = 8
#pragma HLS interface m_axi port = m2 offset = direct bundle = read_data_port_m2 num_write_outstanding = 1 max_read_burst_length = 8
#pragma HLS interface m_axi port = prod offset = direct bundle = write_data_port num_read_outstanding = 1 max_write_burst_length = 8

    int i, j, k;
    TYPE mult;

    //Using static in HW doesn't make sense. Just for checking result for running the kernel in cpu mode.
    static TYPE _m1[row_size][col_size];
    //#pragma HLS RESOURCE variable = _m1 CORE = RAM_2P
    //#pragma HLS array_partition variable = _m1 dim = 1 cyclic factor = 8
    //#pragma HLS array_partition variable = _m1 dim = 2 complete
    static TYPE _m2[row_size][col_size];
    //#pragma HLS RESOURCE variable = _m2 CORE = RAM_2P
    //#pragma HLS array_partition variable = _m2 dim = 1 complete
    //#pragma HLS array_partition variable = _m2 dim = 2 cyclic factor = 8
    static TYPE _prod[row_size][col_size];
    //#pragma HLS RESOURCE variable = _prod CORE = RAM_2P
    //#pragma HLS array_partition variable = _prod dim = 2 complete
    //#pragma HLS array_partition variable = _prod dim = 1 cyclic factor = 8

#pragma HLS DATAFLOW

    if (cmd == __READ__)
    {
    bulk_read_m1:
        for (int rr = 0; rr < row_size; rr++)
        {
            for (int cc = 0; cc < col_size; cc++)
            {
#pragma HLS PIPELINE
                uint32_t abslt_addr = rr * col_size + cc;
                _m1[rr][cc] = m1[abslt_addr];
                _m2[rr][cc] = m2[abslt_addr];
            }
        }
    }
    else if (cmd == __COMPUTE__)
    {
    outer:
        for (i = 0; i < row_size; i++)
        {
        middle:
            for (j = 0; j < col_size; j++)
            {
#pragma HLS PIPELINE
                TYPE sum = 0;
            inner:
                for (k = 0; k < row_size; k++)
                {
                    mult = _m1[i][k] * _m2[k][j];
                    sum += mult;
                }
                _prod[i][j] = sum;
            }
        }
    }
    else
    {
    bulk_write:
        for (int row = 0; row < row_size; row++)
        {
            for (int col = 0; col < col_size; col++)
            {
#pragma HLS PIPELINE
                prod[row * col_size + col] = _prod[row][col];
            }
        }
    }
}