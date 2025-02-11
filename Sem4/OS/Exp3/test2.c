#include<stdio.h>


void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
int main()
{
    int arr[] = {2,6,7,8, 1,3,5,9};
    int p1 = 0;
    int p2 = 4;

    for (p1; p1 <= 4; p1++)
    {
        if (arr[p1] > arr[p2])
        {
            swap(&arr[p1],&arr[p2]);
        }
    }
     
    while (arr[p2+1] < arr[p2])
    {
        swap(&arr[p2],&arr[p2+1]);
        p2++;
    }
    for(int i=3;i<7;i++){
       if (arr[i]>arr[i+1]){
        swap(&arr[i], &arr[i+1]);
        }
    }
    for (size_t i = 0; i < 8; i++)
    {
        printf("%d ",arr[i]);
    }
    printf("\n");
    
    
}