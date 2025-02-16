#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *source_code_name = argv[1];
    int len = strlen(source_code_name);

    char output_file_name[100];
    memcpy(output_file_name, source_code_name, len);

    output_file_name[len - 2] = '\0';

    pid_t pid = fork();
    wait(NULL);
    
    if (pid == 0)
        execl("/usr/bin/gcc", "gcc", source_code_name, "-o", output_file_name,NULL);
    else
        execl(output_file_name, "NULL");
}
