#include<stdio.h>

struct array
{
    int arr[1000];
    int length;
    int (*get)(struct array, int);
    int (*set)(struct array, int, int);
};

int get(struct array A, int i) {
    return A.arr[i-1];
}
void set(struct array A, int i, int val)
{
    A.arr[i - 1] = val;
}
#define ARRAY(NAME, ARR, SIZE) struct array NAME = {.arr = (ARR), .length=SIZE, .get=get, .set=set};


void knapsack(int weight[], int knapsack);

int main()
{
    int n = 4;
    ARRAY(weight, ({1,2,3,4};), 4);
    struct array price = {.arr = {4, 2, 3, 1}, .length = 4, .get = get};
    struct array A = {.arr={1,2,3},.length=3,.get = get};
    printf("%d",A.get(A, 3));
    return 0;
}
