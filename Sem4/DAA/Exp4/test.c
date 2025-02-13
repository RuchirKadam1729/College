#include<stdio.h>
#define LEN(A) sizeof(A)/sizeof(A[0])
#define MAX(X,Y) X >= Y ? X : Y
#define PRINT(X) printf("%d\n", X)
#define PRINTARR(A)                                       \
    for (size_t i = 0; i < sizeof(A) / sizeof(A[0]); i++) \
    {                                                     \
        printf("%d ", A[i]);                              \
    }                                                     \
    printf("\n")

#undef REST
#define REST price, weight, memo, choice
int memoized_max_price_aux(int W, int N, int price[N], int weight[N], int memo[W + 1][N], int choice[W + 1][N]);
int memoized_max_price(int W, int N, int price[N], int weight[N], int memo[W + 1][N], int choice[W + 1][N])
{
    for (int i = 0; i <= W; i++)
    {
        for (int j = 0; j < N; j++)
        {
            memo[i][j] = -1;
            choice[i][j] = 0;
        }
    }
    return memoized_max_price_aux(W, N-1, REST);
}
int memoized_max_price_aux(int w, int k, int price[k], int weight[k], int memo[w + 1][k], int choice[w + 1][k])
{
    if (k < 0 || w == 0)
    {
        choice[w][k] = 0;
        return 0;
    }
    if (memo[w][k] != -1)
    {
        return memo[w][k];
    }

    int case1 = price[k] + memoized_max_price_aux(w - weight[k], k - 1, REST);
    int case2 = memoized_max_price_aux(w, k - 1, REST);

    if (weight[k] > w)
    {
        memo[w][k] = case2;
    }
    else
    {
        memo[w][k] = MAX(case1,case2);
    }
    choice[w][k] = weight[k] > w || case2 >= case1 ? 0 : 1;
    return memo[w][k];
}

void print_table(int rows, int cols, int table[rows][cols], int cellw);

int main()
{
    int price[] = {60, 100, 120};
    int weight[] = {10, 20, 30};
    int N = LEN(price);
    int W = 40;
    int choice[W + 1][N];
    int memo[W + 1][N];
    
    printf("%d\n",memoized_max_price(W,N,REST));
    
    PRINTARR(choice[W]);
    printf("\n");

    print_table(W+1, N, memo, 5);
    printf("\n");
    
}

void print_table(int rows, int cols, int table[rows][cols], int cellw)
{
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++)
        {

            if (table[i][j] == -1)
            {
                printf("%*s%s", cellw - 1, " ", "_");
            }
            else
            {
                char buffer[10];
                int width = sprintf(buffer, "%d", table[i][j]);
                printf("%*s%s", cellw - width, " ", buffer);
            }
        }
        printf("\n");
    }
}