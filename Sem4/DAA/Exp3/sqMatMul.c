#include<stdio.h>
#include<stdlib.h>
struct Matrix
{
    int **a;
    int rows;
    int cols;
};

struct Matrix MatrixMultiply(struct Matrix A, struct Matrix B) {
    if (A.cols != B.rows)
    {
        printf("incompatible dimensions!\n");
    }
    else
    {
        int **mat = malloc(A.rows * sizeof(int*));

        for(int i = 0; i < A.rows; i++) 
            mat[i] = malloc(B.cols * sizeof(int));
        
        
        struct Matrix C = {.a=mat, .rows=A.rows, .cols=B.cols};

        for (int i = 0; i < A.rows; i++)
            for (int j = 0; j < B.cols; j++) {
                
                C.a[i][j] = 0;
                for (int k = 0; k < A.rows; i++)
                    C.a[i][j] += A.a[i][k]*B.a[k][j];
            }

        return C;
    }
    
}

void printMatrix(struct Matrix M) 
{
    for (size_t i = 0; i < M.rows; i++)
    {
        for (size_t j = 0; j < M.cols; j++)
        {
            printf("%d ",M.a[i][j]);
        }
        printf("\n");
    }
    
}

int main() 
{
    struct Matrix A = {.rows=2, .cols=2};
    for (size_t i = 0; i < A.rows; i++)
    {
        for (size_t j = 0; j < A.cols; j++)
        {
            A.a[i][j] = rand() % 2;
        }
    }
    printMatrix(A);
    struct Matrix B = {.rows=2, .cols=2};
    for (size_t i = 0; i < B.rows; i++)
    {
        for (size_t j = 0; j < B.cols; j++)
        {
            B.a[i][j] = rand() % 2;
        }
    }
    printMatrix(B);
    
}