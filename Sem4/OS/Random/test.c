#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int
main (int argc, char *argv[])
{
    printf("proc1 exec proc2\n");
    execl("/home/ubuntu/Documents/Sem4/OS/Random/test2","test2",NULL);
        
    printf("After execl\n");
    return 0;
}



