#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/types.h>
int main()
{
    pid_t pid = fork();

    if (pid > 0)
    { // Parent process
        printf("Parent PID: %d; Child PID: %d\n", getpid(), pid);
        printf("Parent sleeping (zombie time)...\n");
        sleep(30); // Sleep long enough to observe the zombie
        printf("Parent waking up...\n");
        printf("Parent exiting...\n");
    }
    else if (pid == 0)
    { // Child process
        printf("Child exiting...\n");
        exit(0); // Child exits immediately
    }
}