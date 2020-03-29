/* Assignment 1 source code */

// #include <omp.h>
#include <stdio.h>
#include "libs/pagerank.h"

int main(int argc, char *argv[])
{


    
    //reading number of pages from terminal
    uint numpg = (argc > 1) ? atoi(argv[1]) : 16;


    printf("-------Sparse Matrix Test-----------------------\n\n");

    // create the Sparse matrix S
    DMatrix *tmp = initDMatrix(numpg);
    SMatrix *S = initSMatrix(numpg, tmp->data);
    destroyDMatrix(tmp);
    Vector* pgrkV2 = initVector(numpg);

    // display the S matrix
    printf("S matrix = \n");
    printSMatrix(S);

    //prints pagerank vector before matvec
    printf("pagerank vector before web surfing\n");
    printDMatrix(pgrkV2);

    // apply matvec with dampening on for 1000 iterations
    for (uint iter = 0; iter < K; ++iter)
        matVecSp(S, pgrkV2, pgrkV2);
    // matVec(mymat, myvec, myvec);

    if (numpg <= 16)
    { // print the page rank vector is small
        printf("pagerank vector after web surfing\n");
        printDMatrix(pgrkV2);
    }

    // display lowest and highest page ranks
    printf("page rank results: \n");
    minmaxPageRank(pgrkV2);

    // garbage management
    destroySMatrix(S);
    destroyDMatrix(pgrkV2);



    printf("-------Dense Matrix Test-----------------------\n\n");
    // create the H matrix
    DMatrix *H = initDMatrix(numpg);

    // create and initialize at the pagerank Vector
    Vector *pgrkV = initVector(numpg);

    // display the H matrix
    printDMatrix(H);

    // DMatrix *G = dampen(H);

    // printf("Dampened Matrix\n");
    // printDMatrix(G);

    //prints pagerank vector before matvec
    printf("pagerank vector before web surfing\n");
    printDMatrix(pgrkV);

    dampen(H);
    
    // apply matvec with dampening on for 1000 iterations
    for (uint iter = 0; iter < K; ++iter) {
        
        matVec(H, pgrkV, pgrkV); // parallelized matVecDampn
        
    }

    if (numpg <= 16)
    { // print the page rank vector is small
        printf("pagerank vector after web surfing\n");
        printDMatrix(pgrkV);
    }

    // display lowest and highest page ranks
    minmaxPageRank(pgrkV);

    // garbage management
    destroyDMatrix(H);
    destroyDMatrix(pgrkV);

    

    return 0;
}