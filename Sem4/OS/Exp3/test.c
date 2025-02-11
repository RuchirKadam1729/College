#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    fork();
    wait(NULL);
    fork();
    wait(NULL);
    fork();
    wait(NULL);
    fork();
    wait(NULL);
    printf("%d <- %d\n", getpid(), getppid());
}