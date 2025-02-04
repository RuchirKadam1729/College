#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int abs(int x)
{
    x < 0 ? -x : x;
}
int globalVarNum = 50 ;
int randInRange(int start, int end) 
{
    srand(clock());
    return (rand()%(end-start+1)) + start;
}
void generateNum() 
{
    globalVarNum = randInRange(0,100);
}
enum HotCold {HOT,COLD,WIN};
enum HotCold acceptGuess(int guess, int lastGuess) 
{
    return (abs(guess-globalVarNum) < abs(lastGuess-globalVarNum) ? HOT : COLD);
}
