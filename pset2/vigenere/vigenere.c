#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

bool notaword(string key);
string cipher(string text, string key);

int main(int argc, char* argv[])
{
    string key;
    string plaintext;
    
    
    if (argc != 2)
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    else
        key = argv[1];
    
    if ( notaword(key) )
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    else
    {
        printf("plaintext: ");
        plaintext = get_string();
        printf("ciphertext: %s\n", cipher(plaintext, key));
    }
}    

//returns true if the keyword is not alphabetic only
bool notaword(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if ( !isalpha(key[i]))
            return true;
    }
    return false;
}

//returns the cipher string
string cipher(string text, string key)
{
    int textlen = strlen(text);
    int keylen = strlen(key);
    int keys[keylen];
    
    //fill the array of numbers of keys
    for (int i = 0; i < keylen; i++)
    {
        char c = key[i];
        if ( isupper(c))
            keys[i] = (int) c - 65;
        if ( islower(c))
            keys[i] = (int) c - 97;
    }
    
    char c;
    int ckey, control;
    
    for (int i = 0, j = 0; i < textlen; i++)
    {
        c = text[i];
        ckey = keys[j];
        //the variable to be sure that the cipher text keeps being alphabetic
        control = c + ckey;
        
        if ( isalpha(c) )
        {
            if ( isupper(c) )
            {
                if (control > (int) 'Z')
                    c = control - 26;
                else
                    c = control;
            }
            if ( islower(c) )
            {
                if (control > (int) 'z')
                    c = control - 26;
                else
                    c = control;
            }
            text[i] = c;
            j++;
            if ( j >= keylen)
                j = 0;
        }
    }
    return text;
}