#include <stdio.h>
#include"print_table.c"

int main()
{
    int table[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    print_table(3,3,table,2);
    return 0;
}
