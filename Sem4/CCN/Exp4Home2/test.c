#include <stdio.h>
#include "mystringutils.c"

int main(int argc, char const *argv[])
{
    char *str1 = "hi yo";
    char *str2 = "hi";
    char *p = contains(str1, str2);
    printf("%s", p);
    return 0;
}
