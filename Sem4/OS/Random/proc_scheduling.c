#include <stdio.h>

int main()
{
    int
        priority[4] = {10, 20, 30, 40},
        procedure[4] = {0, 1, 2, 3},
        AT[4] = {0, 1, 2, 4},
        BT[4] = {5, 2, 2, 1},
        CT[4] = {12, 8, 4, 5},
        TAT[4],
        WT[4],
        count = 4;

    for (size_t i = 0; i < count; i++)
    {
        WT[i] = CT[i] - AT[i] - BT[i];
        TAT[i] = CT[i] - AT[i];
    }
}