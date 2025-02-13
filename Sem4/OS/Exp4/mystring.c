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
