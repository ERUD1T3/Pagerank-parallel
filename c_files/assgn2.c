/* Assignment 2 source code */

#include <omp.h>
#include <stdio.h>
#include "pagerank.h"

int main(int argc, char *argv[])
{
    //reading number of pages from terminal
    uint numpg = (argc > 1) ? atoi(argv[1]) : 16;

    // create the Sparse matrix S
    DMatrix *tmp = initDMatrix(numpg);
    SMatrix *S = initSMatrix(numpg, tmp->data);
    destroyDMatrix(tmp);

    // create and initialize at the pagerank Vector
    Vector *pgrkV = initVector(numpg);

    // display the S matrix
    printSMatrix(S);

    //prints pagerank vector before matvec
    printDMatrix(pgrkV);

    // apply matvec with dampening on for 1000 iterations
    for (uint iter = 0; iter < K; ++iter)
        matVecSp(S, pgrkV, pgrkV);
    // matVec(mymat, myvec, myvec);

    if (numpg <= 16)
        printDMatrix(pgrkV);

    // display lowest and highest page ranks
    minmaxPageRank(pgrkV);

    // garbage management
    destroySMatrix(S);
    destroyDMatrix(pgrkV);

    return 0;
}