#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void playHvC(int trials) {
    int secret;
    do {
        printf("Enter the secret key (1-100): ");
        scanf("%d", &secret);
    } while (secret < 1 || secret > 100);

    int low = 1, high = 100;
    int guess;
    
    for (int i = 0; i < trials; i++) {
        guess = (low + high) / 2;
        printf("\nComputer's guess %d/%d: %d\n", i+1, trials, guess);
        
        if (guess == secret) {
            printf("Correct! Computer wins!\n");
            return;
        }
        if (guess < secret) {
            printf("Computer's response: Adjusting to HIGHER numbers\n");
            low = guess + 1;
        } else {
            printf("Computer's response: Adjusting to LOWER numbers\n");
            high = guess - 1;
        }
    }
    printf("\nComputer failed! The secret number was %d. You win!\n", secret);
}
