#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>


void *pthread_random(void *arg)
{
    int iterations = *(int *)arg;
    for (int i = iterations; i >= 0; i--)
    {
        sleep(1);
        printf("%d\n", i);
    }
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_random;
    int iterations = 5;
    pthread_create(&thread_random, NULL, pthread_random, (void*)&iterations);
    pthread_join(thread_random, NULL);
}