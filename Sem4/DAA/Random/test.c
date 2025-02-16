#include <stdio.h>
#include <stdlib.h>
#define MAX(X, Y) ((X) >= (Y)) ? (X) : (Y)
struct array
{
    int *arr;
    int length;
};
typedef struct array arr;

int max(arr A, int p, int q)
{
    if (p == q)
        return A.arr[p];
    else
        return MAX(max(A, p, (p + q) / 2), max(A, (p + q) / 2 + 1, q));
}
int max2(arr A)
{
    int max = A.arr[0];
    for (size_t i = 0; i < A.length; i++)
    {
        if (max < A.arr[i])
            max = A.arr[i];
    }
    return max;
}
int main()
{
    int A_arr[500000];
    int count = sizeof(A_arr) / sizeof(A_arr[0]);
    for (size_t i = 0; i < count; i++)
    {
        A_arr[i] = rand();
    }

    arr A = {.arr = A_arr, .length = count};
    printf("%d\n", max(A, 0, A.length - 1));
    printf("%d\n", max2(A));
}