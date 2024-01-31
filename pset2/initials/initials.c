#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void get_initials(string s);

int main(void)
{
    string s = get_string();
    if (s != NULL)
    {
        get_initials(s);
    }
}

//function to print initials in upper case
void get_initials(string s)
{
    int n = strlen(s);
    //check the 1st char of the string
    //print it if is a letter
    if (isalpha(s[0]))
    {
        printf("%c", toupper(s[0]));
    }
    
    //print the next char after the space (ascii=32)
    //if it is a letter
    for (int i = 1; i < n; i++)
    {
       char c = s[i];
       if ( (int) s[i-1] == 32 && isalpha(c) )
       {
           printf("%c", toupper(c));
       }
    }
    printf("\n");
}

