#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct thread_args
{
    int var1;
    int *var2;
};

void *thread_routine(void *arg)
{
    struct thread_args thread_args = *(struct thread_args *)(arg);
    if (thread_args.var1 == 0)
    {
        *thread_args.var2 = 1;
        pthread_exit(NULL);
    }

    pthread_t thread;
    int result = 0;
    struct thread_args thread_args2 = {.var1 = thread_args.var1 - 1, .var2 = &result};

    pthread_create(&thread, NULL, thread_routine, (void *)&thread_args2);
    pthread_join(thread, NULL);
    *thread_args.var2 = thread_args.var1 * *thread_args2.var2;
    pthread_exit(NULL);
}

int fact(int n)
{
    if (n == 0)
    {
        return 1;
    }
    return n * fact(n - 1);
}

int main(int argc, char const *argv[])
{
    for (size_t i = 1; i <= 100000; i += 1)
    {
        pthread_t thread;
        int result = 0;
        struct thread_args thread_args = {.var1 = i, .var2 = &result};
        int a = clock();

        pthread_create(&thread, NULL, thread_routine, (void *)&thread_args);
        pthread_join(thread, NULL);

        double t = (clock() - a) / (double)CLOCKS_PER_SEC;
        printf("[Thread]: %d. Time: %lf\n", result, t);

        int b = clock();
        result = fact(i);
        t = (clock() - b) / (double)CLOCKS_PER_SEC;
        printf("[Recur]: %d. Time: %lf\n", result, t);
    }

    return 0;
}
