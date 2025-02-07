#include<unistd.h>
#include<wait.h>
#include<sys/types.h>
int main()
{
    pid_t p_id = fork();
    wait(NULL);

    if (p_id==0) execl("/usr/bin/gcc","gcc","exec1.c","-o","exec1",NULL);
    else execl("exec1","exec1",NULL);
}   