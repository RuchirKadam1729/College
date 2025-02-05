#include<stdio.h>
#include<stdlib.h>
#include<time.h>
struct Matrix
{
    int **a;
    int rows;
    int cols;
};

int **matrixPtr(int rows, int cols)
{
    int **mat = malloc(rows * sizeof(int*));

        for(int i = 0; i < rows; i++) 
            mat[i] = malloc(cols * sizeof(int));
    return mat;
}

struct Matrix MatrixMultiply(struct Matrix A, struct Matrix B) {
    if (A.cols != B.rows)
    {
        printf("incompatible dimensions!\n");
    }
    else
    {
        struct Matrix C = {.a=matrixPtr(A.rows, B.cols), .rows=A.rows, .cols=B.cols};

        for (int i = 0; i < A.rows; i++)
            for (int j = 0; j < B.cols; j++) {
                
                C.a[i][j] = 0;
                for (int k = 0; k < A.cols; k++)
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
    srand(clock());
    struct Matrix A = {.a=matrixPtr(3,4), .rows=3, .cols=4};
    for (size_t i = 0; i < A.rows; i++)
    {
        for (size_t j = 0; j < A.cols; j++)
        {
            A.a[i][j] = rand() % 10;
        }
    }
    printMatrix(A);
    printf("\n");

    struct Matrix B = {.a=matrixPtr(4,5), .rows=4, .cols=5};
    for (size_t i = 0; i < B.rows; i++)
    {
        for (size_t j = 0; j < B.cols; j++)
        {
            B.a[i][j] = rand() % 10;
        }
    }
    printMatrix(B);
    printf("\n");

    struct Matrix C = MatrixMultiply(A, B);
    printMatrix(C);
    printf("\n");
    
    
}