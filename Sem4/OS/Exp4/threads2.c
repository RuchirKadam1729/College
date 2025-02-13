#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include<time.h>


void *pthread_random(void *arg)
{
    srand(clock());
    int iterations = *(int *)arg;
    for (size_t i = 0; i < iterations; i++)
    {
        printf("%d\n", rand() % 99 + 1);
    }
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_random;
    int iterations = 10;
    pthread_create(&thread_random, NULL, pthread_random, (void*)&iterations);
    pthread_join(thread_random, NULL);
}