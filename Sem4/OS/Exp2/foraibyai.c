#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DEFAULT_LOW 1
#define DEFAULT_HIGH 100

typedef enum { HvH, HvC, CvC } GameMode;
typedef enum { TOO_LOW, TOO_HIGH, CORRECT } GuessResult;

typedef struct {
    int low;
    int high;
    int prev_guess;
    int prev_distance;
} Computer;

// Prototypes
void play_game(GameMode mode, int trials, int range_low, int range_high);
void HvH_game(int trials, int low, int high);
void HvC_game(int trials, int low, int high);
void CvC_game(int trials, int low, int high);
int get_valid_input(const char* prompt, int min, int max);
int computer_guess(Computer *c);
void update_computer(Computer *c, int guess, int secret);
void initialize_computer(Computer *c, int low, int high);

int main() {
    srand(time(NULL));
    
    printf("Welcome to the Number Guessing Game!\n");
    
    // Get game mode
    GameMode mode = get_valid_input(
        "Choose mode (0: HvH, 1: HvC, 2: CvC): ", 0, 2);
    
    // Get number of trials
    int trials = get_valid_input(
        "Enter number of trials: ", 1, 1000);
    
    // Get range
    int low = get_valid_input(
        "Enter range low: ", -1000, 1000);
    int high = get_valid_input(
        "Enter range high: ", low + 1, 1000);

    play_game(mode, trials, low, high);
    
    return 0;
}

void play_game(GameMode mode, int trials, int range_low, int range_high) {
    switch (mode) {
        case HvH:
            HvH_game(trials, range_low, range_high);
            break;
        case HvC:
            HvC_game(trials, range_low, range_high);
            break;
        case CvC:
            CvC_game(trials, range_low, range_high);
            break;
    }
}

void HvH_game(int trials, int low, int high) {
    int secret = get_valid_input("Player 1, enter secret number: ", low, high);
    
    for (int i = 0; i < trials; i++) {
        int guess = get_valid_input("Player 2, enter guess: ", low, high);
        
        if (guess == secret) {
            printf("Correct! Player 2 wins in %d guesses!\n", i+1);
            return;
        }
        
        printf("Too %s\n", guess < secret ? "low" : "high");
    }
    
    printf("Out of trials! Secret was %d\n", secret);
}

void HvC_game(int trials, int low, int high) {
    int secret = get_valid_input("Enter secret number: ", low, high);
    Computer comp;
    initialize_computer(&comp, low, high);

    for (int i = 0; i < trials; i++) {
        int guess = computer_guess(&comp);
        printf("Computer guesses: %d\n", guess);

        if (guess == secret) {
            printf("Computer wins in %d guesses!\n", i+1);
            return;
        }

        update_computer(&comp, guess, secret);
        printf("Too %s\n", guess < secret ? "low" : "high");
    }
    
    printf("Computer failed! Secret was %d\n", secret);
}

void CvC_game(int trials, int low, int high) {
    Computer comp1, comp2;
    initialize_computer(&comp1, low, high);
    initialize_computer(&comp2, low, high);
    
    comp1.prev_guess = low + rand() % (high - low + 1);
    comp2.prev_guess = low + rand() % (high - low + 1);

    for (int i = 0; i < trials; i++) {
        // Computer 1's turn
        int guess1 = computer_guess(&comp1);
        printf("Computer 1 guesses: %d\n", guess1);
        
        if (guess1 == comp2.prev_guess) {
            printf("Computer 1 wins!\n");
            return;
        }
        update_computer(&comp1, guess1, comp2.prev_guess);

        // Computer 2's turn
        int guess2 = computer_guess(&comp2);
        printf("Computer 2 guesses: %d\n", guess2);
        
        if (guess2 == comp1.prev_guess) {
            printf("Computer 2 wins!\n");
            return;
        }
        update_computer(&comp2, guess2, comp1.prev_guess);
    }
    
    printf("Both computers failed after %d trials!\n", trials);
}

int computer_guess(Computer *c) {
    // Binary search with hot/cold adaptation
    int guess = (c->low + c->high) / 2;
    
    // Add slight randomization to avoid infinite loops
    if (rand() % 5 == 0 && c->high - c->low > 1) {
        guess += rand() % 3 - 1;
    }
    
    return guess;
}

void update_computer(Computer *c, int guess, int secret) {
    int current_distance = abs(guess - secret);
    
    if (c->prev_guess != -1) {
        // Adaptive range adjustment based on hot/cold
        if (current_distance < c->prev_distance) {
            if (guess > c->prev_guess) {
                c->low = (c->low + guess) / 2;
            } else {
                c->high = (c->high + guess) / 2;
            }
        } else {
            if (guess < secret) {
                c->low = guess + 1;
            } else {
                c->high = guess - 1;
            }
        }
    }

    // Update computer state
    c->prev_guess = guess;
    c->prev_distance = current_distance;
}

void initialize_computer(Computer *c, int low, int high) {
    c->low = low;
    c->high = high;
    c->prev_guess = -1;
    c->prev_distance = __INT_MAX__;
}

int get_valid_input(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        
        return value;
    }
}