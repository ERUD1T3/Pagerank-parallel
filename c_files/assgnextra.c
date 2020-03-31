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

    uint npp = numpg/ numprocs;

    printf("Worker %d/%d ready to roll, numpg= %d\n", pid, numprocs, numpg);

    // create the Sparse matrix S
    DMatrix *tmpH = initDMatrixV(npp, numpg, 0.0);
    fillDMatrixMultProc(pid, npp, numpg, tmpH);
    SMatrix *S = initSMatrixP(numpg, tmpH->numRow, tmpH->numCol, tmpH->data);
    destroyDMatrix(tmpH);
    Vector *pgrkV = initVectorP(npp, numpg);

    // // display the S matrix
    // printf("pid = %d and here is my S matrix\n", pid);
    // printSMatrix(S);

    // //prints pagerank vector before matvec
    // printf("pid = %d and pagerank vector before web surfing\n", pid);
    // printDMatrix(pgrkV);

    

    // // apply matvec with dampening on for 1000 iterations
    for (uint iter = 0; iter < K; ++iter) {

        // allGather pgrkV to multiply to matvec local H
        double* tmp = malloc(sizeof(double)*npp);

        for(uint i = 0; i < npp; ++i) {
            tmp[i] = pgrkV->data[i][0];
        }

        // MPI_Gather(...)
        double* total = (double *)malloc(sizeof(double) * numpg);
    
        MPI_Allgather(tmp, npp, MPI_DOUBLE, total, npp, MPI_DOUBLE, MPI_COMM_WORLD);

        Vector* totalV = initVectorV(numpg, 0.0);
        for(uint i = 0; i < numpg; ++i) {
            totalV->data[i][0] = total[i];
        }

        pgrkV = matVecSp(S, totalV, npp);
        // printf("pagerank after iter %d \n", iter);
        // printDMatrix(pgrkV);

        destroyDMatrix(totalV);
        free(tmp);
        free(total);
    }




    double* total = NULL;
    if(pid == 0) total = malloc(sizeof(double)*numpg);
    

    double* tmp = malloc(sizeof(double)*npp);

    for(uint i = 0; i < npp; ++i) tmp[i] = pgrkV->data[i][0];
    
    // MPI_Gather(...)
    MPI_Gather(tmp, npp, MPI_DOUBLE, total, npp, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if(pid == 0) {
        printf("pagerank vector after web surfing\n");

        Vector* totalV = initVectorV(numpg, 0.0);
        for(uint i = 0; i < numpg; ++i) {
            totalV->data[i][0] = total[i];
        }
        
        if(numpg <= 16) printDMatrix(totalV);
        minmaxPageRank(totalV);
        destroyDMatrix(totalV);
        free(total);
    }
    free(tmp);
   

    // garbage management
    destroySMatrix(S);
    destroyDMatrix(pgrkV);
    MPI_Finalize();

    return 0;
}
