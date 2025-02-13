#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct array {
    int *arr;
    int length;
} arr;

void *pthread_avg(void *arg)
{
    arr A = *(arr*)(arg);
    int sum = 0;
    for (size_t i = 0; i < A.length; i++)
        sum += A.arr[i];
    
    printf("avg: %.2lf\n", sum/(double)A.length);   
}

void *pthread_max(void *arg)
{
    arr A = *(arr *)(arg);
    int max = A.arr[0];
    for (size_t i = 0; i < A.length; i++)
        if (max < A.arr[i])
            max = A.arr[i];
         
    printf("max: %d\n", max);
}

int compare(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}
void *pthread_median(void *arg)
{
    arr A = *(arr *)(arg);
    int arr[A.length];
    for (size_t i = 0; i < A.length; i++)
        arr[i] = A.arr[i];
    
    qsort(arr, A.length, sizeof(int), compare);
    
    printf("median: %d\n", arr[A.length/2]);
}

int main()
{
    int A_arr[8] = {2,1,5,3,6,2,5,4}; 
    arr A = {.arr = A_arr, .length=sizeof(A_arr)/sizeof(A_arr[0])};

    pthread_t thread_avg, thread_max, thread_median;

    pthread_create(&thread_avg, NULL, pthread_avg, (void*)(&A));
    pthread_create(&thread_max, NULL, pthread_max, (void *)(&A));
    pthread_create(&thread_median, NULL, pthread_median, (void *)(&A));
    pthread_join(thread_avg, NULL);
    pthread_join(thread_max, NULL);
    pthread_join(thread_median, NULL);
}