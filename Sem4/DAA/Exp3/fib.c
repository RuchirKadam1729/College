#include<stdio.h>

int main() 
{
    int f[1000];
    f[0] = 1;
    f[1] = 1;
    for (size_t i = 2; i < 1000; i++)
    {
        f[i] = f[i-1] + f[i-2];
        printf("%d ",f[i]);
    }   
    
    
}