/* Assignment 1 source code */

#include <stdio.h>
#include "mpi.h"
#include "libs/pagerankMPI.h"

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    MPI_Status status;
    uint pid, numprocs; // process id and number of processes

    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    

    //reading number of pages from terminal
    uint numpg = (argc > 1) ? atoi(argv[1]) : 16;

    uint npp = numpg / numprocs;

    // create the H matrix
    // DMatrix *H = initDMatrix(numpg);
    DMatrix *H = initDMatrixP(npp, numpg);

    // create and initialize at the pagerank Vector
    Vector *pgrkV = initVectorP(npp, numpg);

    // display the H matrix
    printf("pid = %d and here is my H matrixs\n", pid);
    printDMatrix(H);

    //prints pagerank vector before matvec
    printf("pid = %d and pagerank vector before web surfing\n", pid);
    printDMatrix(pgrkV);

    // apply matvec with dampening on for 1000 iterations
    // for (uint iter = 0; iter < K; ++iter) 
    // {
    //     pgrkV = matVec(H, pgrkV, pid, numprocs); // parallelized matVecDampn
    //     printf("pagerank after iter %d\n", iter);
    //     printDMatrix(pgrkV);
    // }

    // if (numpg <= 16)
    // { // print the page rank vector is small
    //     printf("pagerank vector after web surfing\n");
    //     printDMatrix(pgrkV);
    // }

    // // display lowest and highest page ranks
    // minmaxPageRank(pgrkV);

    // garbage management
    destroyDMatrix(H);
    destroyDMatrix(pgrkV);

    MPI_Finalize();

    return 0;
}