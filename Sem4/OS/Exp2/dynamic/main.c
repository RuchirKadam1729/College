#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include"playCvC.h"
#include"playHvC.h"
#include"playHvH.h"

int main() {
    int mode, trials;

    do {
        printf("Select mode:\n1. Human vs Human\n2. Human vs Computer\n3. Computer vs Computer\n");
        scanf("%d", &mode);
    } while (mode < 1 || mode > 3);

    do {
        printf("Enter number of trials (at least 1): ");
        scanf("%d", &trials);
    } while (trials < 1);

    switch(mode) {
        case 1:
            playHvH(trials);
            break;
        case 2:
            playHvC(trials);
            break;
        case 3:
            playCvC(trials);
            break;
        default:
            printf("Invalid mode.\n");
    }

    return 0;
}