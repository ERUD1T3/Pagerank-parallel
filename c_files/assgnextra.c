/* Assignment 2 source code */

#include <stdio.h>
#include "libs/pagerankMPI.h"
#include "mpi.h"

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    MPI_Status status;
    int pid, numprocs; // process id and number of processes

    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    
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
        matVecSp(S, pgrkV, pgrkV, pid, numprocs);
    // matVec(mymat, myvec, myvec);




    if (numpg <= 16)
        printDMatrix(pgrkV);

    // display lowest and highest page ranks
    minmaxPageRank(pgrkV);

    // garbage management
    destroySMatrix(S);
    destroyDMatrix(pgrkV);



    MPI_Finalize();

    return 0;
}