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

int max_price_bottom_up(int N, int W, int price[], int weight[], int max_price[][W + 1], int choice[][W + 1])
{
    for (size_t k = 0; k < N; k++)
        for (size_t w = 0; w <= W; w++)
        {
            max_price[k][w] = -1;
            choice[k][w] = 0;
        }

    for (size_t w = 0; w <= W; w++)
    {
        max_price[0][w] = (w >= weight[0]) ? price[0] : 0;
        choice[0][w] = (w >= weight[0]) ? 1 : 0;
    }

    for (size_t k = 1; k < N; k++)
        for (size_t w = 0; w <= W; w++)
        {

            int case1 = price[k] + max_price[k - 1][w - weight[k]];
            int case2 = max_price[k - 1][w];

            if (weight[k] > w)
                max_price[k][w] = case2;
            else
                max_price[k][w] = MAX(case1, case2);

            choice[k][w] = (weight[k] > w) || (case2 > case1) ? 0 : 1;
        }

    return max_price[N - 1][W];
}

int main()
{
    int price[] = {60, 100, 120, 78, 34, 72, 59, 28, 25, 46, 36, 90};
    int weight[] = {10, 20, 30, 40, 50, 60, 1  , 80, 70, 30, 35, 23};
    int N = LEN(price);
    int W = 80;

    int max_price[N][W + 1];
    int choice[N][W + 1];
    

    int max_priceWN = max_price_bottom_up(N, W, price, weight, max_price, choice);

    int choice_for_W[N];
    int remaining_weight = W;
    for (int i = N - 1; i >= 0; i--)
    {
        if (choice[i][remaining_weight])
        {
            choice_for_W[i] = 1;
            remaining_weight -= weight[i];
        }
        else
            choice_for_W[i] = 0;
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

    printf("memoized array:\n");
    print_table(N, W + 1, max_price, 4);
    printf("\n");
}