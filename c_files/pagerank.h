#ifndef PAGERANK_H
#define PAGERANK_H

#include <stdio.h>
#include <stdlib.h>

//const
#define DPN_FACTOR .15 // dampening factor
#define NUM_ITERS 1000 // number of matvec iterations

//typedefs
typedef unsigned int uint;
typedef struct dmatrix DMatrix;
typedef struct smatrix SMatrix;
typedef struct dmatrix Vector; // typedef for Vector based on DMatrix

//function prototypes
DMatrix *initDMatrix(uint numpg);                       //initialize new dense matrix
Vector *initVector(uint numpg);                         // intialize a new vector
SMatrix *initSMatrix(uint numpg);                       // initialize a new sparse matrix
void matvecMul(DMatrix *mat, Vector *vec, Vector *res); // multiply compatible matrix and vector
void printDMatrix(DMatrix *dmat);
void fillDMatrix(DMatrix *mat, float val);
void destroyDMatrix(DMatrix *mat);
void vecNormalize(Vector *vec); // normalize values of surfer values
void pagerank(uint numpg);

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
    uint numpg;
    // size of vectors respectively
    uint nnzelsN, colidxN, nnzpsumN;
    float *nnzels, // stores non zero elements of the matrix
        *colidx,   // column indices of elements in nnzeroels
        *nnzpsum;  //partial sum of the num zero elements

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

void matvecMul(DMatrix *mat, Vector *vec, Vector *res)
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

void vecNormalize(Vector *vec)
{
    // normalize the content of vector to sum up to one
    float sum = 0;
    for (uint r = 0; r < vec->numRow; ++r)
        sum += vec->data[r][0];

    for (uint r = 0; r < vec->numRow; ++r)
        vec->data[r][0] /= sum;
}

void Dense2Sparse(DMatrix *dmat, SMatrix *smat)
{
    //convert Dense Matrix to Sparse Matrix
}

void pagerank(uint numpg)
{
    DMatrix *mymat = initDMatrix(numpg);
    Vector *myvec = initVector(numpg);
    // Vector *res = initVector(10);

    // fillDMatrix(res, 0);

    printDMatrix(mymat);
    printDMatrix(myvec);
    // printDMatrix(res);

    for (uint iter = 0; iter < 2; ++iter)
    {

        matvecMul(mymat, myvec, myvec);
        printDMatrix(myvec);
    }

    destroyDMatrix(mymat);
    destroyDMatrix(myvec);
}
#endif // DENSE_MAT