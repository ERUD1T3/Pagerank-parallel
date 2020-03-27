/* Assignment 1 source code */

// #include <omp.h>
#include <stdio.h>
#include "libs/pagerankOMP.h"

int main(int argc, char *argv[])
{
    //reading number of pages from terminal
    uint numpg = (argc > 1) ? atoi(argv[1]) : 16;

    printf("number of pages = %d\n", numpg);

    // create the H matrix
    DMatrix *H = initDMatrix(numpg);
    Vector *pgrkV = initVector(numpg);

    // display the H matrix
    printf("H matrix = \n");
    printDMatrix(H);

    dampen(H);

    printf("Dampened Matrix = \n");
    printDMatrix(H);

    //prints pagerank vector before matvec
    printf("pagerank vector before web surfing\n");
    printDMatrix(pgrkV);

    // apply matvec with dampening on for 1000 iterations
    for (uint iter = 0; iter < K; ++iter)
        matVec(H, pgrkV, pgrkV); // parallelized matVecDampn
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
    destroyDMatrix(H);
    destroyDMatrix(pgrkV);

    return 0;
}