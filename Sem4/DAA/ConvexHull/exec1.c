#include<unistd.h>
#include<wait.h>
#include<sys/types.h>
int main()
{
    pid_t p_id = fork();
    wait(NULL);

    if (p_id==0) execl("/usr/bin/gcc","gcc","convex.c","-o","convex","-lraylib","-lGL","-lpthread","-lrt","-ldl","-lrt","-lX11","-Ihome/ubuntu/raylib/src","-lm",NULL);
    else execl("convex","convex",NULL);
}