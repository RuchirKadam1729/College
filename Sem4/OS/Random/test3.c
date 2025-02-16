#include <stdio.h>

void recur(int a, int b, int A[3][3])
{
    if (a == 10)
        return;

    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    recur(a + 1, b + 1, A);
}

int main()
{
    int A[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    printf("\n");
    recur(3, 3, A);
}