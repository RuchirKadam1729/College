#include <stdio.h>
#include <stdlib.h>
#include "print_table.c"
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
#define MAXW 50

int memo_accesses = 0;

int max_price_recur(int k, int w, int price[], int weight[])
{
    if (k == 0 || w == 0)
        return 0;

    int case1 = price[k] + max_price_recur(k - 1, w - weight[k], price, weight);
    int case2 = max_price_recur(k - 1, w, price, weight);

    if (w < weight[k])
        return case2;
    else
        return MAX(case1, case2);
}

int memoized_max_price(int k, int w, int price[], int weight[], int max_price[][MAXW + 1], int choice[][MAXW + 1]);

int max_price_top_down(int N, int W, int price[], int weight[], int max_price[][MAXW + 1], int choice[][MAXW + 1])
{
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j <= MAXW; j++)
        {
            max_price[i][j] = -1;
            choice[i][j] = 0;
        }

    return memoized_max_price(N - 1, W, price, weight, max_price, choice);
}

int memoized_max_price(int k, int w, int price[], int weight[], int max_price[][MAXW + 1], int choice[][MAXW + 1])
{
    if (k == 0 || w == 0)
    {
        max_price[k][w] = 0;
        choice[k][w] = 0;
        return 0;
    }

    if (max_price[k][w] >= 0)
    {
        memo_accesses += 1;
        return max_price[k][w];
    }

    int case1 = price[k] + memoized_max_price(k - 1, w - weight[k], price, weight, max_price, choice);

    int case2 = memoized_max_price(k - 1, w, price, weight, max_price, choice);

    if (w < weight[k])
        max_price[k][w] = case2;
    else
        max_price[k][w] = MAX(case1, case2);

    choice[k][w] = w < weight[k] || case2 > case1 ? 0 : 1;
    return max_price[k][w];
}

int main()
{
    int price[] = {60, 100, 120};
    int weight[] = {10, 20, 30};
    int N = 3;
    int W = 50;

    int max_price[N][MAXW + 1];
    int choice[N][MAXW + 1];
    

    int max_priceWN = max_price_top_down(N, W, price, weight, max_price, choice);

    int choice_for_W[N];
    int remaining_weight = W;
    for (int k = N - 1; k >= 0; k--)
    {
        if (choice[k][remaining_weight])
        {
            choice_for_W[k] = 1;
            remaining_weight -= weight[k];
        }
        else
            choice_for_W[k] = 0;
    }

    for (size_t i = 0; i < N; i++)
        printf("Item #%d:\t(%d kg)\t(Rs. %d)\t%s\n", i + 1, weight[i], price[i], choice_for_W[i] ? "<--" : "");

    printf("sack capacity: ");
    PRINT(W);
    printf("max price: ");
    PRINT(max_priceWN);
    printf("choice: ");
    PRINTARR(choice_for_W);

    printf("Select items ");
    for (size_t i = 0; i < N; i++)
        if (choice_for_W[i] == 1)
            printf("#%d, ", i + 1);
    printf("\b\b \n");

    printf("memo accesses: %d\n", memo_accesses);

    printf("memoized array:\n");
    print_table(N, W + 1, max_price, 4);
    printf("\n");
}