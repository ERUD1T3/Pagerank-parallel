/* Assignment 2 source code */

// #include <omp.h>
#include <stdio.h>
#include "libs/pagerankOMP.h"

int main(int argc, char *argv[])
{
    //reading number of pages from terminal
    uint numpg = (argc > 1) ? atoi(argv[1]) : 16;
    printf("number of pages = %d\n", numpg);

    // create the Sparse matrix S
    DMatrix *tmp = initDMatrix(numpg);
    SMatrix *S = initSMatrix(numpg, tmp->data);
    destroyDMatrix(tmp);
    Vector *pgrkV = initVector(numpg);

    // display the S matrix
    printf("S matrix = \n");
    printSMatrix(S);

    //prints pagerank vector before matvec
    printf("pagerank vector before web surfing\n");
    printDMatrix(pgrkV);

    // apply matvec with dampening on for 1000 iterations
    for (uint iter = 0; iter < K; ++iter)
        matVecSp(S, pgrkV, pgrkV);
    // matVec(mymat, myvec, myvec);

    if (numpg <= 16)
    { // print the page rank vector is small
        printf("pagerank vector after web surfing\n");
        printDMatrix(pgrkV);
    }

    // display lowest and highest page ranks
    printf("page rank results: \n");
    minmaxPageRank(pgrkV);

    // garbage management
    destroySMatrix(S);
    destroyDMatrix(pgrkV);

    return 0;
}