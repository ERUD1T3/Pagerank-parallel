// pagerank with MPI multithreading

#ifndef PAGERANKMPI_H
#define PAGERANKMPI_H


#include <stdio.h>
#include <stdlib.h>
#include "dmatrix.h"
#include "smatrix.h"
#include "mpi.h"

//constants
#define Q .15  // dampening factor
#define K 1000 // number of matvec iterations

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//function prototypes

/* non -parallel */
void minmaxPageRank(Vector *vec);
DMatrix *dampen(DMatrix *H);    

/* parallel */
void vecNormalize(Vector *vec, uint pid, uint numprocs);                      // normalize values of surfer values
void matVec(DMatrix *mat, Vector *vec, Vector *res, uint pid, uint numprocs); // multiply compatible matrix and vector
void matVecSp(SMatrix *mat, Vector *vec, Vector *res, uint pid, uint numprocs);
                     // transform H matrix into G (dampened) matrix

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// definition of dense matrix object
void minmaxPageRank(Vector *vec)
{
    // return the max and min values in the vector, as well as their indices
     double minval = vec->data[0][0], maxval = vec->data[0][0];
    uint minidx = 0, maxidx = 0;

    for (uint r = 0; r < vec->numRow; ++r)
    {
        if (vec->data[r][0] >= maxval)
        {
            maxval = vec->data[r][0];
            maxidx = r;
        }

        if (vec->data[r][0] <= minval)
        {
            minval = vec->data[r][0];
            minidx = r;
        }
    }

    printf("X[min = %d] = %.2lf | X[max = %d] = %.2lf\n",
           minidx, minval, maxidx, maxval);
}

DMatrix *dampen(DMatrix *mat)
{
    // multiply compatible matrix and vector

    uint numpg = mat->numRow;

    for (uint r = 0; r < mat->numRow; ++r)
        for (uint c = 0; c < mat->numCol; ++c)
            mat->data[r][c] = Q / numpg + (1.0 - Q) * mat->data[r][c];

    return mat;
}



/////////////////////////////////////////////////////////////////////
void vecNormalize(Vector *vec, uint pid, uint numprocs)
{
    // normalize the content of vector to sum up to one
    // parallelized vecNormalize
    double sum = 0;

    for (uint r = 0; r < vec->numRow; ++r)
        sum += vec->data[r][0];

    for (uint r = 0; r < vec->numRow; ++r)
        vec->data[r][0] /= sum;
}

void matVec(DMatrix *mat, Vector *vec, Vector *res, uint pid, uint numprocs)
{
    // multiply compatible matrix and vector

    // uint numprocs, pid;
    double tmp;
    


    for (uint r = pid + 1; r < mat->numRow; r += numprocs)
    {
        
        tmp = 0.0;
        // res->data[r][0] = 0.0;
  
        for (uint c = 0; c < mat->numCol; ++c)
        {
            tmp += mat->data[r][c] * vec->data[c][0];
        }

        // MPI_Reduce(&tmp, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        res->data[r][0] = tmp;
    }

    vecNormalize(res, pid, numprocs);
    
}

void matVecSp(SMatrix *mat, Vector *vec, Vector *res, uint pid, uint numprocs)
{
   
    for (uint r = 0; r < mat->rowidxN - 1; ++r)
    {
        double tmp = 0.0;
        // res->data[r][0] = 0.0;

        for (uint c = mat->rowidx[r]; c < mat->rowidx[r + 1]; ++c)
        {
            tmp += mat->nnzels[c] * vec->data[mat->colidx[c]][0];
        }

        // res->data[r][0] = tmp;
        res->data[r][0] = tmp * (1-Q) + Q / vec->numRow;
    }

    vecNormalize(res, pid, numprocs);
}


#endif // DENSE_MAT