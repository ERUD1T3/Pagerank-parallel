#ifndef PAGERANK_H
#define PAGERANK_H

#include <stdio.h>
#include <stdlib.h>
#include "sllist.h"

//const
#define Q .15  // dampening factor
#define K 1000 // number of matvec iterations

//typedefs
typedef unsigned int uint;
typedef struct dmatrix DMatrix;
typedef struct smatrix SMatrix;
typedef struct dmatrix Vector; // typedef for Vector based on DMatrix

//function prototypes
DMatrix *initDMatrix(uint numpg);                 //initialize new dense matrix
Vector *initVector(uint numpg);                   // intialize a new vector
SMatrix *initSMatrix(uint nnzelsN, uint rowidxN); // initialize a new sparse matrix
void destroySMatrix(SMatrix *mat);
void matVec(DMatrix *mat, Vector *vec, Vector *res); // multiply compatible matrix and vector
void printDMatrix(DMatrix *dmat);
void fillDMatrix(DMatrix *mat, float val);
void destroyDMatrix(DMatrix *mat);
void vecNormalize(Vector *vec); // normalize values of surfer values
void pagerank(uint numpg);
SMatrix *Dense2Sparse(DMatrix *dmat);
void printSMatrix(SMatrix *smat);
void minmaxPageRank(Vector *vec);
void matVecSp(SMatrix *mat, Vector *vec, Vector *res);
void matVecDampn(DMatrix *mat, Vector *vec, Vector *res); // multiply compatible matrix and vector

// definition of dense matrix object
struct dmatrix
{
    //square matrix
    uint numRow, numCol;
    // probabilities contained in the matrix
    float **data;
};

// definition of sparse matrix using CSR format
struct smatrix
{
    // size of vectors respectively
    uint nnzelsN, colidxN, rowidxN;
    // float *nnzels; // stores non zero elements of the matrix
    SLList *nnzels,
        *colidx;  // column indices of elements in nnzeroels
    uint *rowidx; //partial sum of the num zero elements

}; //

DMatrix *initDMatrix(uint numpg)
{
    //initialize a new Dense matrix for rangking algorithm

    // create pointer to matrix
    DMatrix *matrix = (DMatrix *)malloc(sizeof(DMatrix));
    // matrix->numpg = numpg;
    matrix->numCol = numpg;
    matrix->numRow = numpg;

    // setting up data matrix to zeros
    matrix->data = (float **)malloc(numpg * sizeof(float *));
    for (uint r = 0; r < numpg; ++r)
        matrix->data[r] = (float *)malloc(numpg * sizeof(float));

    fillDMatrix(matrix, 1.0 / numpg);
    return matrix;
}

void destroyDMatrix(DMatrix *mat)
{
    // detroy matrix object and free its memory
    for (uint r = 0; r < mat->numRow; ++r)
        free(mat->data[r]);

    free(mat->data);
    free(mat);
}

Vector *initVector(uint numpg)
{
    //initialize the surf vector
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    // vec->numpg = numpg;
    vec->numRow = numpg;
    vec->numCol = 1;

    // setting up data matrix to zeros
    vec->data = (float **)malloc(numpg * sizeof(float *));
    for (uint r = 0; r < numpg; ++r)
        vec->data[r] = (float *)malloc(sizeof(float));

    vec->data[0][0] = 1; // full probability to be on page 1
    return vec;
}

void printDMatrix(DMatrix *dmat)
{
    // print the dense matrix
    printf("[\n");
    for (uint r = 0; r < dmat->numRow; ++r)
    {
        printf("[");
        for (uint c = 0; c < dmat->numCol; ++c)
        {
            printf(" %.2f", dmat->data[r][c]);
        }
        printf(" ]\n");
    }
    printf("]\n");
}

void fillDMatrix(DMatrix *mat, float val)
{
    // fillDMatrix the content of a DMatrix to val specified
    for (uint r = 0; r < mat->numRow; ++r)
        for (uint c = 0; c < mat->numCol; ++c)
            mat->data[r][c] = val;
}

void matVec(DMatrix *mat, Vector *vec, Vector *res)
{
    // multiply compatible matrix and vector

    float tmp = 0.0;
    for (uint r = 0; r < mat->numRow; ++r)
    {
        // res->data[r][0] = 0.0;
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
    float tmp = 0.0;
    for (uint r = 0; r < mat->rowidxN - 1; ++r)
    {
        // res->data[r][0] = 0.0;
        for (uint c = mat->rowidx[r]; c < mat->rowidx[r + 1]; ++c)
        {
            tmp += *(float *)(getAt(mat->nnzels, c)) * vec->data[*(uint *)getAt(mat->colidx, c)][0];
        }
        res->data[r][0] = tmp;
    }

    vecNormalize(res);
}

void vecNormalize(Vector *vec)
{
    // normalize the content of vector to sum up to one
    float sum = 0;
    for (uint r = 0; r < vec->numRow; ++r)
        sum += vec->data[r][0];

    for (uint r = 0; r < vec->numRow; ++r)
        vec->data[r][0] /= sum;
}

SMatrix *initSMatrix(uint nnzelsN, uint rowsize)
{
    SMatrix *newmat = (SMatrix *)malloc(sizeof(SMatrix));
    newmat->nnzelsN = nnzelsN;
    newmat->colidxN = nnzelsN;
    newmat->rowidxN = rowsize + 1;
    // newmat->nnzels = (float *)malloc(nnzelsN * sizeof(float));
    newmat->rowidx = (uint *)malloc((rowsize + 1) * sizeof(uint));
    // newmat->colidx = (uint *)malloc(nnzelsN * sizeof(uint));
    newmat->nnzels = initList();
    newmat->colidx = initList();

    return newmat;
}

void destroySMatrix(SMatrix *mat)
{
    destroyList(mat->nnzels);
    destroyList(mat->colidx);
    free(mat->rowidx);
    free(mat);
}

SMatrix *Dense2Sparse(DMatrix *dmat)
{
    //convert Dense Matrix to Sparse Matrix
    uint iter = 0;
    for (uint r = 0; r < dmat->numRow; ++r)
        for (uint c = 0; c < dmat->numCol; ++c)
            if (dmat->data[r][c] != 0.0)
                iter++;

    SMatrix *res = initSMatrix(iter, dmat->numRow);

    uint rowidx_counter = 0;
    for (uint r = 0; r < dmat->numRow; ++r)
    {
        for (uint c = 0; c < dmat->numCol; ++c)
        {
            if (dmat->data[r][c] != 0.0)
            {
                if (c == 0)
                { // begin a row
                    res->rowidx[rowidx_counter++] = res->nnzels->size;
                }
                // storing non zero elements
                pushback(res->nnzels, &(dmat->data[r][c]));
                // storing the columns of the non zero elements
                uint *colptr = (uint *)malloc(sizeof(uint));
                *colptr = c;
                pushback(res->colidx, colptr);
                // adding index of row
            }
        }
    }

    res->rowidx[res->rowidxN - 1] = iter;

    return res;
}

void printSMatrix(SMatrix *smat)
{
    //prints the SMatrix
    printf("\non zero elements:");
    printlist(smat->nnzels, 'f');
    printf("\ncolumn indices");
    printlist(smat->colidx, 'i');
    printf("\nrow indices ");
    for (uint i = 0; i < smat->rowidxN; ++i)
        printf("%d ", smat->rowidx[i]);

    printf("\n");
}

void pagerank(uint numpg)
{
    DMatrix *mymat = initDMatrix(numpg);
    Vector *myvec = initVector(numpg);
    // SMatrix *mysmat = Dense2Sparse(mymat);
    // Vector *res = initVector(10);

    // fillDMatrix(res, 0);

    printDMatrix(mymat);
    printDMatrix(myvec);
    // printSMatrix(mysmat);
    // printDMatrix(res);

    for (uint iter = 0; iter < K; ++iter)
    {

        // matVec(mymat, myvec, myvec);
        matVecDampn(mymat, myvec, myvec);
        // matVecSp(mysmat, myvec, myvec);
        // printDMatrix(myvec);
    }

    printDMatrix(myvec);
    minmaxPageRank(myvec);

    destroyDMatrix(mymat);
    destroyDMatrix(myvec);
    // destroySMatrix(mysmat);
}

void minmaxPageRank(Vector *vec)
{
    // return the max and min values in the vector, as well as their indices
    float minval = vec->data[0][0], maxval = vec->data[0][0];
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

    printf("X[min = %d] = %.2f | X[max = %d] = %.2f\n", minidx, minval, maxidx, maxval);
}

void matVecDampn(DMatrix *mat, Vector *vec, Vector *res)
{
    // multiply compatible matrix and vector

    float tmp = 0.0;
    for (uint r = 0; r < mat->numRow; ++r)
    {
        // res->data[r][0] = 0.0;
        for (uint c = 0; c < mat->numCol; ++c)
        {
            tmp += mat->data[r][c] * vec->data[c][0];
        }

        uint numpg = vec->numRow;
        // following is the expression for a row of Gx
        res->data[r][0] = tmp * (1 - Q) + (Q + (1 - Q) * vec->data[numpg - 1][0]) / numpg;
    }

    vecNormalize(res);
}
#endif // DENSE_MAT