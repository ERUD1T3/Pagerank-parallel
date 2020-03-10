#include <omp.h>
#include <stdio.h>
#include "pagerank.h"

int main(int argc, char *argv[])
{
    uint numpg = (argc > 1) ? atoi(argv[1]) : 16;
    DMatrix *mymat = initDMatrix(numpg);
    Vector *myvec = initVector(numpg);
    // SMatrix *mysmat = Dense2Sparse(mymat);
    // Vector *res = initVector(10);

    // printDMatrix(mymat);
    // printDMatrix(myvec);
    // printSMatrix(mysmat);
    // printDMatrix(res);

    for (uint iter = 0; iter < K; ++iter)
    {

        // matVec(mymat, myvec, myvec);
        matVecDampn(mymat, myvec, myvec);
        // matVecSp(mysmat, myvec, myvec);
        // printDMatrix(myvec);
    }

    if (numpg <= 16)
        printDMatrix(myvec);
    minmaxPageRank(myvec);

    // garbage management
    destroyDMatrix(mymat);
    destroyDMatrix(myvec);
    // destroySMatrix(mysmat);
    return 0;
}