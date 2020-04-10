#include "nw.h"

#define MATCH_SCORE 1
#define MISMATCH_SCORE -1
#define GAP_SCORE -1

#define ALIGN '\\'
#define SKIPA '^'
#define SKIPB '<'

#define MAX(A, B) (((A) > (B)) ? (A) : (B))

void needwun(char SEQA[ALEN], char SEQB[BLEN],
             char alignedA[ALEN + BLEN], char alignedB[ALEN + BLEN],
             int M[(ALEN + 1) * (BLEN + 1)], char ptr[(ALEN + 1) * (BLEN + 1)], command_t cmd)
{

    int score, up_left, up, left, max;
    int row, row_up, r;
    int a_idx, b_idx;
    int a_str_idx, b_str_idx;

    static int _M[(ALEN + 1) * (BLEN + 1)];
    static char _ptr[(ALEN + 1) * (BLEN + 1)];

    static char _SEQA[ALEN];
    static char _SEQB[BLEN];

    static char _alignedA[ALEN + BLEN];
    static char _alignedB[ALEN + BLEN];

    if (cmd == __READ__)
    {
    bulk_read_1:
        for (int ii = 0; ii < ALEN; ii++)
        {
#pragma HLS PIPELINE
            _SEQA[ii] = SEQA[ii];
            _SEQB[ii] = SEQB[ii];
        }

    BULK_READ_for_INOUT_SIGNAL:
        for (int f = 0; f < ((ALEN + 1) * (BLEN + 1)); f++)
        {
#pragma HLS PIPELINE
            _M[f] = M[f];
            _ptr[f] = ptr[f];
        }
    }
    else if (cmd == __COMPUTE__)
    {

    //----------------------------------------------------------------------------
    init_row:
        for (a_idx = 0; a_idx < (ALEN + 1); a_idx++)
        {
            _M[a_idx] = a_idx * GAP_SCORE;
        }
    init_col:
        for (b_idx = 0; b_idx < (BLEN + 1); b_idx++)
        {
            _M[b_idx * (ALEN + 1)] = b_idx * GAP_SCORE;
        }

    // Matrix filling loop
    fill_out:
        for (b_idx = 1; b_idx < (BLEN + 1); b_idx++)
        {
        fill_in:
            for (a_idx = 1; a_idx < (ALEN + 1); a_idx++)
            {
                if (_SEQA[a_idx - 1] == _SEQB[b_idx - 1])
                {
                    score = MATCH_SCORE;
                }
                else
                {
                    score = MISMATCH_SCORE;
                }

                row_up = (b_idx - 1) * (ALEN + 1);
                row = (b_idx) * (ALEN + 1);

                up_left = _M[row_up + (a_idx - 1)] + score;
                up = _M[row_up + (a_idx)] + GAP_SCORE;
                left = _M[row + (a_idx - 1)] + GAP_SCORE;

                max = MAX(up_left, MAX(up, left));

                _M[row + a_idx] = max;
                if (max == left)
                {
                    _ptr[row + a_idx] = SKIPB;
                }
                else if (max == up)
                {
                    _ptr[row + a_idx] = SKIPA;
                }
                else
                {
                    _ptr[row + a_idx] = ALIGN;
                }
            }
        }

        // TraceBack (n.b. aligned sequences are backwards to avoid string appending)
        a_idx = ALEN;
        b_idx = BLEN;
        a_str_idx = 0;
        b_str_idx = 0;

    trace:
        while (a_idx > 0 || b_idx > 0)
        {
            r = b_idx * (ALEN + 1);
            if (_ptr[r + a_idx] == ALIGN)
            {
                _alignedA[a_str_idx++] = _SEQA[a_idx - 1];
                _alignedB[b_str_idx++] = _SEQB[b_idx - 1];
                a_idx--;
                b_idx--;
            }
            else if (_ptr[r + a_idx] == SKIPB)
            {
                _alignedA[a_str_idx++] = _SEQA[a_idx - 1];
                _alignedB[b_str_idx++] = '-';
                a_idx--;
            }
            else
            { // SKIPA
                _alignedA[a_str_idx++] = '-';
                _alignedB[b_str_idx++] = _SEQB[b_idx - 1];
                b_idx--;
            }
        }

    // Pad the result
    pad_a:
        for (; a_str_idx < ALEN + BLEN; a_str_idx++)
        {
            _alignedA[a_str_idx] = '_';
        }
    pad_b:
        for (; b_str_idx < ALEN + BLEN; b_str_idx++)
        {
            _alignedB[b_str_idx] = '_';
        }
    }
    else
    {
        //----------------------------------------------------------------------------

    BULK_WRITE_for_INOUT_SIGNAL:
        for (int ff = 0; ff < ((ALEN + 1) * (BLEN + 1)); ff++)
        {
#pragma HLS PIPELINE
            M[ff] = _M[ff];
            ptr[ff] = _ptr[ff];
        }

    bulk_write_2:
        for (int fff = 0; fff < ALEN + BLEN; fff++)
        {
#pragma HLS PIPELINE
            alignedA[fff] = _alignedA[fff];
            alignedB[fff] = _alignedB[fff];
        }
    }
}
