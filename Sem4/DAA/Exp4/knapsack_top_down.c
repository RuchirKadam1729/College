#include <stdio.h>
#include <stdlib.h>
#define LEN(A) sizeof(A) / sizeof(A[0])
#define MAX(X, Y) (X >= Y ? X : Y)
#define SIZE 500
#define PRINT(X) printf("%d\n", X)
#define PRINTARR(A)                                       \
    for (size_t i = 0; i < sizeof(A) / sizeof(A[0]); i++) \
    {                                                     \
        printf("%d ", A[i]);                              \
    }                                                     \
    printf("\n")

int knapsack()