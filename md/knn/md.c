/*
Implemenataion based on:
A. Danalis, G. Marin, C. McCurdy, J. S. Meredith, P. C. Roth, K. Spafford, V. Tipparaju, and J. S. Vetter.
The scalable heterogeneous computing (shoc) benchmark suite.
In Proceedings of the 3rd Workshop on General-Purpose Computation on Graphics Processing Units, 2010.
*/

#include "md.h"

void md_kernel(TYPE force_x[nAtoms],
               TYPE force_y[nAtoms],
               TYPE force_z[nAtoms],
               TYPE position_x[nAtoms],
               TYPE position_y[nAtoms],
               TYPE position_z[nAtoms],
               int32_t NL[nAtoms * maxNeighbors])
{
    TYPE delx, dely, delz, r2inv;
    TYPE r6inv, potential, force, j_x, j_y, j_z;
    TYPE i_x, i_y, i_z, fx, fy, fz;

    int32_t i, j, jidx;

    TYPE _force_x[nAtoms];
    TYPE _force_y[nAtoms];
    TYPE _force_z[nAtoms];
    TYPE _position_x[nAtoms];
    TYPE _position_y[nAtoms];
    TYPE _position_z[nAtoms];
    int32_t _NL[nAtoms * maxNeighbors];

bulk_read_1:
    for (int ii = 0; ii < nAtoms; ii++)
    {
        _position_x[ii] = position_x[ii];
        _position_y[ii] = position_y[ii];
        _position_z[ii] = position_z[ii];
    }

bulk_read_2:
    for (int iii = 0; iii < nAtoms * maxNeighbors; iii++)
    {
        _NL[iii] = NL[iii];
    }

loop_i:
    for (i = 0; i < nAtoms; i++)
    {
        i_x = _position_x[i];
        i_y = _position_y[i];
        i_z = _position_z[i];
        fx = 0;
        fy = 0;
        fz = 0;
    loop_j:
        for (j = 0; j < maxNeighbors; j++)
        {
            // Get neighbor
            jidx = _NL[i * maxNeighbors + j];
            // Look up x,y,z positions
            j_x = _position_x[jidx];
            j_y = _position_y[jidx];
            j_z = _position_z[jidx];
            // Calc distance
            delx = i_x - j_x;
            dely = i_y - j_y;
            delz = i_z - j_z;
            r2inv = 1.0 / (delx * delx + dely * dely + delz * delz);
            // Assume no cutoff and aways account for all nodes in area
            r6inv = r2inv * r2inv * r2inv;
            potential = r6inv * (lj1 * r6inv - lj2);
            // Sum changes in force
            force = r2inv * potential;
            fx += delx * force;
            fy += dely * force;
            fz += delz * force;
        }
        //Update forces after all neighbors accounted for.
        _force_x[i] = fx;
        _force_y[i] = fy;
        _force_z[i] = fz;
        //printf("dF=%lf,%lf,%lf\n", fx, fy, fz);
    }

bulk_write:
    for (int jj = 0; jj < nAtoms; jj++)
    {
        force_x[jj] = _force_x[jj];
        force_y[jj] = _force_y[jj];
        force_z[jj] = _force_z[jj];
    }
}
