#include<stdio.h>

int main()
{
    int ***func;
    func[-5][2][3] = 5;

    printf("%d\n",func[-5]);
}