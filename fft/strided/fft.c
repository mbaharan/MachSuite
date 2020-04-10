#include "fft.h"

void fft(double real[FFT_SIZE], double img[FFT_SIZE], double real_twid[FFT_SIZE / 2], double img_twid[FFT_SIZE / 2], command_t cmd)
{

    static double _real[FFT_SIZE];
    static double _img[FFT_SIZE];
    static double _real_twid[FFT_SIZE / 2];
    static double _img_twid[FFT_SIZE / 2];

    int even, odd, span, log, rootindex;
    double temp;
    log = 0;

#pragma HLS DATAFLOW

    if (cmd == __READ__)
    {
        //BULK READ
    bulk_read_1:
        for (int i = 0; i < FFT_SIZE; i++)
        {
#pragma HLS PIPELINE
            _real[i] = real[i];
            _img[i] = img[i];
        }

    bulk_read_2:
        for (int j = 0; j < (FFT_SIZE >> 1); j++)
        {
#pragma HLS PIPELINE
            _real_twid[j] = real_twid[j];
            _img_twid[j] = img_twid[j];
        }
    }
    else if (cmd == __COMPUTE__)
    {
    //KERNEL
    outer:
        for (span = FFT_SIZE >> 1; span; span >>= 1, log++)
        {
        inner:
            for (odd = span; odd < FFT_SIZE; odd++)
            {
                odd |= span;
                even = odd ^ span;

                temp = _real[even] + _real[odd];
                _real[odd] = _real[even] - _real[odd];
                _real[even] = temp;

                temp = _img[even] + _img[odd];
                _img[odd] = _img[even] - _img[odd];
                _img[even] = temp;

                rootindex = (even << log) & (FFT_SIZE - 1);
                if (rootindex)
                {
                    temp = _real_twid[rootindex] * _real[odd] -
                           _img_twid[rootindex] * _img[odd];
                    _img[odd] = _real_twid[rootindex] * _img[odd] +
                                _img_twid[rootindex] * _real[odd];
                    _real[odd] = temp;
                }
            }
        }
    }
    else
    {
    //BULK WRITE
    bulk_write:
        for (int k = 0; k < FFT_SIZE; k++)
        {
#pragma HLS PIPELINE
            real[k] = _real[k];
            img[k] = _img[k];
        }
    }
}
