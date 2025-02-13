#include <stdio.h>
#include "print_table.c"
#define LEN(A) sizeof(A) / sizeof(A[0])
#define MAX(X, Y) X >= Y ? X : Y
#define PRINT(X) printf("%d\n", X)
#define PRINTARR(A)                                       \
    for (size_t i = 0; i < sizeof(A) / sizeof(A[0]); i++) \
    {                                                     \
        printf("%d ", A[i]);                              \
    }                                                     \
    printf("\n")

#undef REST
#define REST price, weight, memo, choice

int memoized_max_price_aux(int W, int k, int price[], int weight[], int memo[][k + 1], int choice[][k + 1]);

int memoized_max_price(int W, int N, int price[], int weight[], int memo[][N], int choice[][N])
{
    for (size_t i = 0; i <= W; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            memo[i][j] = -1;
            choice[i][j] = 0;
        }
    }
    return memoized_max_price_aux(W, N - 1, REST);
}

int memoized_max_price_aux(int w, int k, int price[], int weight[], int memo[][k + 1], int choice[][k + 1])
{
    if (k < 0 || w == 0)
    {
        return 0;
    }
    if (memo[w][k] != 1)
    {
        return memo[w][k];
    }
    else
    {
        int case1 = price[k] + memoized_max_price_aux(w - weight[k], k - 1, REST);
        int case2 = memoized_max_price_aux(w, k - 1, REST);

        if (w < weight[k])
        {
            memo[w][k] = case2;
        }
        else
        {
            memo[w][k] = MAX(case1, case2);
        }
        return memo[w][k];
    }
}

int main()
{
    int price[] = {60, 100, 120};
    int weight[] = {10, 20, 30};
    int N = LEN(price);
    int W = 40;
    int choice[W + 1][N];
    int memo[W + 1][N];

    printf("%d\n", memoized_max_price(W, N, REST));

    PRINTARR(choice[W]);
    printf("\n");

    print_table(W + 1, N, memo, 5);
    printf("\n");
}
