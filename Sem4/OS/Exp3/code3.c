#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    int a = 100;
    pid_t pid = fork();
    printf("\nvalue of a: %d (%s)", a, pid > 0 ? "in parent":"in child");
    printf("\nboth change.");
    if (pid == 0) {
        printf("\nchild: ");
        a = 10;
    }
    else {
        printf("\nparent: ");
        a = 20;
    }
    printf("\n%d\n", a);    
}
