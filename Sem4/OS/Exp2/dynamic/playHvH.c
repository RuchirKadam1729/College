#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void playHvH(int trials) {
    int secret;
    do {
        printf("Player 1, enter the secret key (1-100): ");
        scanf("%d", &secret);
    } while (secret < 1 || secret > 100);

    printf("\n\n\n\n\n\n\n\n\n\n");  // Attempt to hide the secret key input

    int guess;
    for (int i = 0; i < trials; i++) {
        printf("Player 2, enter guess %d/%d: ", i+1, trials);
        scanf("%d", &guess);
        
        if (guess == secret) {
            printf("Correct! Player 2 wins!\n");
            return;
        }
        printf("The secret number is %s\n\n", guess < secret ? "HIGHER" : "LOWER");
    }
    printf("Out of trials! The secret number was %d. Player 1 wins!\n", secret);
}