#include<stdio.h>
#include"board.c"
#include<unistd.h>
#include<stdbool.h>
enum Round {FIRST,SECOND, THIRD};
struct cpuInfo {
    int guess;
    int pivot;
    int subpivot1;
    int subpivot2;
    enum Round round;
    int temp;
};
void printCpu(struct cpuInfo cpu) 
{
    printf("guess = %d; pivot = %d; subPivot1 = %d; subPivot2 = %d; temp = %s\n",cpu.guess,cpu.pivot,cpu.subpivot1,cpu.subpivot2,cpu.temp==0?"HOT":"COLD");
}

struct cpuInfo cpuGuess(struct cpuInfo cpu) 
{
    //have a pivot in middle of line. Have TWO points at any given time on either side of pivot, dividing the left and right lines too.
    //at random, choose one of the subpivots to guess as answer.

    //next round, choose the other subpivot as answer

    //next round, compare your guess with the previous subpivot's guess.
    //if the current guess is cold, prev. guessed subpivot was correct, else current guessed pivot is correct.
    //make current subpivot the new pivot, and create subpivots for it.
    //repeat
    if (cpu.round==FIRST)
    {
        int choice = randInRange(0,1);
        if (choice == 0)
        {
            cpu.guess = cpu.subpivot1;
        }
        else
        {
            cpu.guess = cpu.subpivot2;
        }
        cpu.round = SECOND;
    }
    else if (cpu.round==SECOND)
    {
        int guess = cpu.guess - 2*(cpu.guess - cpu.pivot);
        cpu.temp = acceptGuess(guess, cpu.guess);
        cpu.round = THIRD;
    }
    else 
    {
        if (cpu.temp == HOT)
        {
            cpu.subpivot1 = cpu.guess - abs(cpu.guess - cpu.pivot) / 2;
            cpu.subpivot2 = cpu.guess + abs(cpu.guess - cpu.pivot) / 2;
            cpu.pivot = cpu.guess;
        }
        else if (cpu.temp == COLD)
        {
            cpu.guess -= 2*(cpu.guess - cpu.pivot);

            cpu.subpivot1 = cpu.guess - abs(cpu.guess-cpu.pivot)/2;
            cpu.subpivot2 = cpu.guess + abs(cpu.guess-cpu.pivot)/2;
            cpu.pivot = cpu.guess;
        }
        cpu.round = FIRST;
    }
    
    printCpu(cpu);
    return cpu;
}


int main(int argc, char const *argv[])
{
    struct cpuInfo cpu1 = {0,50,25,75,FIRST,UNKNOWN};

    generateNum();
    printf("actual number is: %d\n",globalVarNum);
    
    int i = 1;
    printf("%d.) ",i++);
    printCpu(cpu1);
    while (cpu1.pivot != globalVarNum)
    {
        printf("%d.) ",i++);
        cpu1 = cpuGuess(cpu1);
        sleep(1);
        //printf("\n");
       
    }
    if (cpu1.pivot == globalVarNum)
    {
        printf("CPU Wins!\n");
    }
    
    
    return 0;
}
