#include <stdio.h>
#include "pagerank.h"

int main(void)
{
    uint numpg = 10;
    DMatrix *mymat = initDMatrix(numpg);
    Vector *myvec = initVector(numpg);
    // SMatrix *mysmat = Dense2Sparse(mymat);
    // Vector *res = initVector(10);

    // fillDMatrix(res, 0);

    float data[10][10] = {
        {0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5},
        {1.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0}};

    fillDMatrixfromData(mymat, data);

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
    return 0;
}