#ifndef PAGERANKOMP_H
#define PAGERANKOMP_H

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "dmatrix.h"
#include "smatrix.h"

//constants
#define Q .15  // dampening factor
#define K 1000 // number of matvec iterations

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//function prototypes

/* non parallel */
void minmaxPageRank(Vector *vec);
DMatrix *dampen(DMatrix *H);                         // transform H matrix into G (dampened) matrix

/* parallel */
void vecNormalize(Vector *vec);                      // normalize values of surfer values
void matVec(DMatrix *mat, Vector *vec, Vector *res); // multiply compatible matrix and vector
void matVecSp(SMatrix *mat, Vector *vec, Vector *res);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// definition of dense matrix object
void minmaxPageRank(Vector *vec)
{
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
            mat->data[r][c] = Q / numpg + (1.0 - Q) * mat->data[r][c] ;

    return mat;
}


///////////////////////////////////////////////////////////////////////////////////////////

void vecNormalize(Vector *vec)
{
    // normalize the content of vector to sum up to one
    // parallelized vecNormalize
    double sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (uint r = 0; r < vec->numRow; ++r)
        sum += vec->data[r][0];

    #pragma omp parallel for
    for (uint r = 0; r < vec->numRow; ++r)
        vec->data[r][0] /= sum;
}

void matVec(DMatrix *mat, Vector *vec, Vector *res)
{
    // multiply compatible matrix and vector

    

    #pragma omp parallel for 
    for (uint r = 0; r < mat->numRow; ++r)
    {
        double tmp = 0.0;
        // res->data[r][0] = 0.0;
        #pragma omp parallel for reduction(+:tmp)
        for (uint c = 0; c < mat->numCol; ++c)
        {
            tmp += mat->data[r][c] * vec->data[c][0];
        }

        res->data[r][0] = tmp;
    }

    vecNormalize(res);
}

void matVecSp(SMatrix *mat, Vector *vec, Vector *res)
{
    #pragma omp parallel for
    for (uint r = 0; r < mat->rowidxN - 1; ++r)
    {
        double tmp = 0.0;
        // res->data[r][0] = 0.0;
        #pragma omp parallel for reduction(+:tmp)
        for (uint c = mat->rowidx[r]; c < mat->rowidx[r + 1]; ++c)
        {
            tmp += mat->nnzels[c] * vec->data[mat->colidx[c]][0];
        }

        // res->data[r][0] = tmp;
        res->data[r][0] = tmp * (1-Q) + Q / vec->numRow;
    }

    vecNormalize(res);
}



#endif // DENSE_MAT
