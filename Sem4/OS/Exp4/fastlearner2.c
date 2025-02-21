#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct job
{
    int i;
    int (*func)(int);
};

struct thread_args
{
    int i;
    struct job task;
    int *var2;
    int *var3;
};

#pragma region queue
struct queue
{
    struct job job[500];
    int front;
    int rear;
};

struct queue queue() { return (struct queue){.front = -1, .rear = -1}; }

int isEmpty(struct queue queue) { return queue.rear == queue.front; }

struct queue enqueue(struct queue queue, struct job job)
{
    queue.job[++queue.rear] = job;
    return queue;
}

struct queue dequeue(struct queue queue, struct job *job)
{
    *job = (queue.job[++queue.front]);
    return queue;
}
#pragma endregion

void *thread_routine(void *arg)
{
    struct thread_args thread_args = *(struct thread_args *)(arg);
    *thread_args.var3 = 1;

    int b4task = clock();
    int secs = rand() % 10;
    thread_args.var2[thread_args.task.i - 1] = thread_args.task.func(secs);
    printf("Thread %d completed Task %d.   (%d secs)\n", thread_args.i, thread_args.task.i, secs);
    *thread_args.var3 = 0;
    pthread_exit(NULL);
}

int func(int a)
{
    sleep(a); // we will imagine doing some work in this time.
    return a * a + 2 * a + 1;
}

int main(int argc, char const *argv[])
{
    struct queue ready_queue = queue();
    struct job task[] =
        {{.i = 1, .func = func},
         {.i = 2, .func = func},
         {.i = 3, .func = func},
         {.i = 4, .func = func},
         {.i = 5, .func = func},
         {.i = 6, .func = func}};

    int ntask = sizeof(task) / sizeof(task[0]);

    for (size_t i = 0; i < ntask; i++)
    {
        ready_queue = enqueue(ready_queue, task[i]);
    }

    int n_threads = 3;
    pthread_t thread_pool[n_threads];
    struct thread_args thread_args[n_threads];

    int taskresult[ntask];
    int isRunning[n_threads];
    for (size_t i = 0; i < n_threads; i++)
    {
        thread_args[i].i = i + 1;
        thread_args[i].var3 = &isRunning[i];
        thread_args[i].var2 = taskresult;
        *thread_args[i].var3 = 0;
    }

    int i = 0;
    while (!isEmpty(ready_queue))
    {
        struct job task;
        ready_queue = dequeue(ready_queue, &task);
        int kthread = 0; // some thread
        while (*thread_args[(kthread = (kthread + 1) % n_threads)].var3 == 1)
            ;
        // now kthread will be the some free thread.
        *thread_args[kthread].var3 = 1;
        thread_args[kthread].task = task;
        pthread_create(&thread_pool[kthread], NULL, thread_routine, &thread_args[kthread]);
        i++;
    }

    for (size_t j = 0; j < n_threads; j++)
        pthread_join(thread_pool[j], NULL);

    printf("\nResults:\n");
    for (size_t i = 0; i < ntask; i++)
        printf("Task %d: %d\n", i + 1, taskresult[i]);

    return 0;
}
