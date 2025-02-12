#include <stdio.h>
#include <limits.h>
#define I(X) ((X) - 1) // 1-indexing
#define MAX(X, Y) ((X) >= (Y) ? (X) : (Y))
#define PRINT(X) printf("%d\n", X);
int memo_accesses = 0;
// recursive max_price to memoize using top-down approach
int max_price(int remaining_weight, int k, int weight[], int price[])
{
    if (remaining_weight == 0 || k == 0)
        return 0;

    int
        case1 = price[I(k)] +
                max_price(remaining_weight - weight[I(k)], k-1, weight, price),

        case2 = max_price(remaining_weight, k-1, weight, price);

    if (remaining_weight < weight[I(k)])
        return case2;
    else
        return MAX(case1, case2);
}


int max_price_bottom_up(int W, int N, int weight[], int price[])
{
    int max_price[1000][1000];
    for (size_t i = 0; i <= 1000; i++)
        for (size_t j = 0; j <= 1000; j++)
            max_price[i][j] = 0;
    

    for (size_t w = 0; w <= W; w++)
        if (weight[I(1)] <= w)
            max_price[w][1] = price[I(1)];

    for (size_t w = 0; w <= W; w++)
    {
        for (size_t k = 2; k <= N; k++)
        {
            int
            case1 = price[I(k)] + max_price[w-weight[I(k)]][k-1],
            case2 = max_price[w][k-1];

            if (weight[I(k)] > w) 
                max_price[w][k] = case2;
            else
                max_price[w][k] = MAX(case1, case2);
            
        }
           
    }

    PRINT(max_price[W][N])
    return max_price[W][N];
}


int main()
{
    int weight[] = {15, 83, 94, 73, 20, 63, 13, 14, 23, 91, 47, 55};
    int price[] = {73, 43, 27, 90, 86, 37, 82, 64, 15, 19, 20, 21};
    int N = sizeof(weight) / sizeof(int);
    int W = 500;

    PRINT(max_price_bottom_up(W, N, weight, price))
    PRINT(memo_accesses);
    return 0;
}
