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
    Vector *pgrkV = initVector(numpg);

    // display the S matrix
    printf("S matrix = \n");
    printSMatrix(S);

    //prints pagerank vector before matvec
    printf("pagerank vector before web surfing\n");
    printDMatrix(pgrkV);



    // apply matvec with dampening on for 1000 iterations
    for (uint iter = 0; iter < K; ++iter) {
        pgrkV = matVecSp(S, pgrkV, pid, numprocs);
        printf("pagerank after iter %d \n", iter);
        printDMatrix(pgrkV);
    }




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



    MPI_Finalize();

    return 0;
}
