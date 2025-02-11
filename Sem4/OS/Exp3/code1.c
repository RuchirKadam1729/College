#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    pid_t b1 = 1, b2 = 1, b3 = 1, b4 = 1;
    
    for(int i = 0; i < 2; i++) {
        if (0 == i) {
            b1 = fork() > 0 ? 1 : 0;
            wait(NULL);
            b2 = fork() > 0 ? 1 : 0;
            wait(NULL);
        }
        else {
            b3 = fork() > 0 ? 1 : 0;
            wait(NULL);
            b4 = fork() > 0 ? 1 : 0;
            wait(NULL);
        }
    }
    fflush(stdout);
    printf("%d %d %d %d: %d <- %d\n",b1,b2,b3,b4, getpid()%100, getppid()%100);
}