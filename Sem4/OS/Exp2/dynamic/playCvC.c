#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void playCvC(int trials) {
    srand(time(0));
    int secret = rand() % 100 + 1;
    printf("Computer 1 has chosen a secret number between 1-100.\n\n");

    int low = 1, high = 100;
    int guess;
    
    for (int i = 0; i < trials; i++) {
        guess = (low + high) / 2;
        printf("Computer 2's guess %d/%d: %d\n", i+1, trials, guess);
        
        if (guess == secret) {
            printf("Correct! Computer 2 wins!\n");
            return;
        }
        if (guess < secret) {
            printf("Computer 2's response: Adjusting to HIGHER numbers\n");
            low = guess + 1;
        } else {
            printf("Computer 2's response: Adjusting to LOWER numbers\n");
            high = guess - 1;
        }
    }
    printf("Computer 2 failed! The secret number was %d. Computer 1 wins!\n", secret);
}