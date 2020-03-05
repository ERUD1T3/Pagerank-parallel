#include <stdio.h>
#include "pgMatrix.h"

int main(void)
{
    DMatrix *mymat = initDMatrix(10);
    Vector *myvec = initVector(10);
    // Vector *res = initVector(10);

    // fillDMatrix(res, 0);

    printDMatrix(mymat);
    printDMatrix(myvec);
    // printDMatrix(res);

    for (uint iter = 0; iter < 2; ++iter)
    {

        matvecMul(mymat, myvec, myvec);
        printDMatrix(myvec);
    }

    return 0;
}