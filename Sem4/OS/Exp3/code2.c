#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
int main()
{
    pid_t pid = fork();
    if(pid == 0) {
        printf("\nhello from child\n");
    }                                                       
    else {
        sleep(1);
        printf("\ngoodbye from parent\n");
    }
}