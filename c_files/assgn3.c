/* Assignment 1 source code */

#include <stdio.h>
#include "mpi.h"
#include "libs/pagerankMPI.h"

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    MPI_Status status;
    int pid, numprocs; // process id and number of processes

    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    

    //reading number of pages from terminal
    uint numpg = (argc > 1) ? atoi(argv[1]) : 16;

    // create the H matrix
    DMatrix *H = initDMatrix(numpg);

    // create and initialize at the pagerank Vector
    Vector *pgrkV = initVector(numpg);

    // display the H matrix
    printDMatrix(H);

    dampen(H);

    printf("Dampened Matrix\n");
    printDMatrix(H);

    //prints pagerank vector before matvec
    printf("pagerank vector before web surfing\n");
    printDMatrix(pgrkV);

    // apply matvec with dampening on for 1000 iterations
    for (uint iter = 0; iter < K; ++iter)
        matVec(H, pgrkV, pgrkV, pid, numprocs); // parallelized matVecDampn
    // matVec(mymat, myvec, myvec);

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

    MPI_Finalize();

    return 0;
}