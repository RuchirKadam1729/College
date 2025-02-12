#include<stdio.h>
#define I(X) ((X)-1) //1-indexing
#define MAX(X,Y) ((X) >= (Y) ? (X) : (Y))
int memo_accesses = 0;
//recursive max_price to memoize using top-down approach
int max_price(int remaining_weight, int weight[], int price[], int k)
{
    if (remaining_weight == 0 || k == 0)
        return 0;
    
    if (remaining_weight < weight[I(k)])
        return max_price(remaining_weight, weight, price, k-1);
    
    else {
        int
        case1 = price[I(k)] + 
                max_price(remaining_weight - weight[I(k)], weight, price, k-1),

        case2 = max_price(remaining_weight, weight, price, k-1); 
            
        return MAX(case1, case2);
    }  
}

max_price_bottomup(int W, int weight[], int price[], int N)
{
    int max_price[1000][1000];
    for (size_t i = 0; i < 1000; i++)
        for (size_t j = 0; j < 1000; j++)
            max_price[i][j] = 1000;
    
}
int main()
{
    int weight[] = {15, 83, 94, 73, 20, 63, 13, 14, 23, 91, 47, 55};
    int price[] =  {73, 43, 27, 90, 86, 37, 82, 64, 15, 19, 20, 21};
    int N = sizeof(weight)/sizeof(int);
    int W = 500;
    #define PRINT(X) printf("%d\n",X);
    PRINT(memoized_max_price(W,weight,price,N))
    PRINT(memo_accesses);
    return 0;
}
