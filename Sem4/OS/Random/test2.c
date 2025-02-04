#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int
main (int argc, char *argv[])
{
    printf("proc2 exec proc1\n");
    execl("/home/ubuntu/Documents/Sem4/OS/Random/test","test",NULL);
        
    printf("After execl\n");
    return 0;
}



