
int equals(char *str1, char *str2)
{
    int i = 0;
#define ISDELIM(c) (c == '\0' || c == ' ' || c == '\n' || c == '\0')
    while (!ISDELIM(str1[i]) && !ISDELIM(str2[i]))
    {
        if (str1[i] != str2[i])
            return 0;
        i++;
    }
    if (!ISDELIM(str1[i]) || !ISDELIM(str2[i]))
        return 0;
    return 1;
}

int current_word_equals(char *str1, char *str2)
{
}

char *contains(char *str1, char *str2)
{
    int i = 0;
    while (str1[i] != '\0')
    {
        if (equals(str2, str1 + i))
            return str1 + i;
        i++;
    }
    return ((void *)0); // NULL coz I cant include stdlib twice;
}