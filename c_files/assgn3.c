/* Assignment 1 source code */

#include <stdio.h>
#include "libs/pagerankMPI.h"

int main(int argc, char *argv[])
{
    //reading number of pages from terminal
    uint numpg = (argc > 1) ? atoi(argv[1]) : 16;

    // create the H matrix
    DMatrix *H = initDMatrix(numpg);

    // create and initialize at the pagerank Vector
    Vector *pgrkV = initVector(numpg);

    // display the H matrix
    printDMatrix(H);

    //prints pagerank vector before matvec
    printDMatrix(pgrkV);

    // apply matvec with dampening on for 1000 iterations
    for (uint iter = 0; iter < K; ++iter)
        matVecDampn(H, pgrkV, pgrkV);
    // matVec(mymat, myvec, myvec);

    if (numpg <= 16)
        printDMatrix(pgrkV);

    // display lowest and highest page ranks
    minmaxPageRank(pgrkV);

    // garbage management
    destroyDMatrix(H);
    destroyDMatrix(pgrkV);

    return 0;
}