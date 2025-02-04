#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int
main (int argc, char *argv[])
{
    printf("before execl\n");
    execl("./","ls","-l",NULL);
    
    printf("After execl\n");
    return 0;
}



