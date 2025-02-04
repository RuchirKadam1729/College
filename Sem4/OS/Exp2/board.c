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
    if (start < end)
    {
        srand(clock());
        return (rand()%(end-start+1)) + start;
    }
    else
    {
        return start;
    }
    
    
    
}
void generateNum() 
{
    globalVarNum = randInRange(0,100);
}
enum HotCold {UNKNOWN,HOT,COLD,WIN};
enum HotCold acceptGuess(int guess, int lastGuess) 
{
    if (guess==globalVarNum)
    {
        return WIN;
    }
    else if (guess==lastGuess)
    {
        return UNKNOWN;
    }
    else
    {
        return (abs(guess-globalVarNum) <= abs(lastGuess-globalVarNum) ? HOT : COLD);
    }
}
