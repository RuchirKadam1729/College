#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/types.h>
int main()
{
    pid_t pid = fork();

    if (pid > 0)
    {
        printf("Parent PID: %d\n", getpid());
        printf("Parent exiting...\n");
    }
    else if (pid == 0)
    {
        sleep(2); // Wait for parent to terminate
        printf("\nChild PID: %d\n", getpid());
        printf("Child's New Parent PID (init): %d\n", getppid()); // Now 1
        // Keep running to show orphan state
        while (1)
        {
            sleep(1);
        }
    }
}