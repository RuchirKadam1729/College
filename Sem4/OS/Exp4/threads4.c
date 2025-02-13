#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct string
{
    char *charAt;
    int length;
} string;

void print(string str)
{
    for (int i = 0; i < str.length; i++)
        printf("%c", str.charAt[i]);
}

#define STR(STRING) {.charAt = STRING, .length = strlen(STRING)};

int equals(string str1, string str2)
{

    if (str1.length != str2.length)
        return 0;

    for (int i = 0; i < str1.length; i++)
        if (str1.charAt[i] != str2.charAt[i])
            return 0;

    return 1;
}

string to_struct(char *str)
{
    return (string){.charAt = str, .length = strlen(str)};
}
int equals_literal(char *str1, char *str2)
{
    string str11 = to_struct(str1);
    string str21 = to_struct(str2);

    return equals(str11, str21);
}

int isdelimiter(char c) {return c==' ' || c=='\n' || c=='\0';}


char *next_word_ptr(char *line)
{
    int i=0;
    while (isdelimiter(line[i]) == 0)
        i++;
    
    if (line[i] == '\n' || line[i] == '\0')
        return NULL;

    while (line[i] == ' ')
        i++;

    if (line[i] == '\n' || line[i] == '\0')
        return NULL;
    else
        return &line[i];
}

string current_word(char *line)
{
    for (size_t i = 0; i < 100; i++)
        if (isdelimiter(line[i]))
            return (string){.charAt = line, .length = i};
}
struct keyword_count
{
    string keyword;
    int count;
};
struct keywordcount_fp_lines
{
    struct keyword_count *keyword_count;
    FILE *fp;
    int lines;
};

void *pthread_routine(void *arg)
{
    struct keywordcount_fp_lines *keywordcount_fp_lines = (struct keywordcount_fp_lines *)arg;

    string keyword = keywordcount_fp_lines->keyword_count->keyword;
    FILE *fp = keywordcount_fp_lines->fp;
    int lines = keywordcount_fp_lines->lines;

    char line[100];
    for (size_t i = 0; i < lines; i++)
    {
        fgets(line, 100, fp);
        char* lineptr = line;
        
        if (equals(current_word(lineptr), keyword))
            keywordcount_fp_lines->keyword_count->count++;
        
        while (next_word_ptr(lineptr) != NULL) {
            lineptr = next_word_ptr(lineptr);

            if (equals(current_word(lineptr), keyword))
                keywordcount_fp_lines->keyword_count->count++;
        }
    }

    pthread_exit(NULL);
}

int main()
{
    FILE *fplines, *fp1, *fp2;
    fplines = fopen("file.txt", "r");
    fp1 = fopen("file.txt", "r");
    fp2 = fopen("file.txt", "r");
    char line[100];
    int lines = 0;

    //first fgets counting all lines in file
    while (fgets(line, 100, fplines))
        lines++;
    
    for (size_t i = 0; i < lines / 2; i++)
        fgets(line, 100, fp2);
    //Now the fp2 ^ will be at lines/2 th line

    string keyword = STR("When"); 
    struct keyword_count keyword_count = {keyword, 0};

    pthread_t thread_half1, thread_half2;
    struct keywordcount_fp_lines keywordcount_fp_lines1 = {.keyword_count = &keyword_count, .fp = fp1, .lines = lines / 2};
    struct keywordcount_fp_lines keywordcount_fp_lines2 = {.keyword_count = &keyword_count, .fp = fp2, .lines = lines - lines / 2};
    pthread_create(&thread_half1, NULL, pthread_routine, (void *)&keywordcount_fp_lines1);
    pthread_create(&thread_half2, NULL, pthread_routine, (void *)&keywordcount_fp_lines2);
    pthread_join(thread_half1, NULL);
    pthread_join(thread_half2, NULL);

    printf("Count of keyword [");
    print(keyword);
    printf("] in document: %d\n", keyword_count.count);
}