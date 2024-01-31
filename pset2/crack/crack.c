#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

#define _XOPEN_SOURCE

void crack(char* hashed);
bool psw_found(char* password, char* hashed);


int main(int argc, char* argv[])
{
    char* hashed;
    //check if the user entered the hash to be cracked
    if ( argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    else
    {
        hashed = argv[1];
    }
    //exit if hashed is NULL
    if ( hashed == NULL)
    {
        printf("Problem");
        return 1;
    }
    
    crack(hashed);
    return 0;
}//main

void crack(char* hashed)
{
    //create an array with the set of characers a-z, A-Z
    const int ALPHABET_LENGTH = 52;
    char alphabet[ALPHABET_LENGTH];
    alphabet[0] = 'a';
    for (int i = 1; i < 26; i++)
        {
            
            alphabet[i] = (char) alphabet[i-1] + 1;
        }
    alphabet[26] = 'A';
    for (int i = 27; i < ALPHABET_LENGTH; i++)
    {
       alphabet[i] = (char) alphabet[i-1] + 1;
    }
    
    
    bool bingo = false; //psw is cracked
    char password[5];
    password[4] = '\0';
    
    //i for the 1st letter, j for the 2nd etc
    for(int i = 0; i < ALPHABET_LENGTH; i++)
    {
        password[0] = alphabet[i];
        if (psw_found(password, hashed))
        {
          bingo = true;
          break;
        }
        
        for(int j = 0; j < ALPHABET_LENGTH; j++)
        {
            password[1] = alphabet[j];
            if (psw_found(password, hashed))
            {
              bingo = true;
              break;
            }
            for(int k = 0; k < ALPHABET_LENGTH; k++)
            {
                password[2] = alphabet[k];
                if (psw_found(password, hashed))
                {
                  bingo = true;
                  break;
                }
                for(int l = 0; l < ALPHABET_LENGTH; l++)
                {
                    password[3] = alphabet[l];
                    if (psw_found(password, hashed))
                    {
                      bingo = true;
                      break;
                    }
                }
                password[3] = '\0';
                if(bingo)
                    break;
            }
            password[2] = '\0';
            if(bingo)
                break;
        }
        password[1] = '\0';
        if(bingo)
            break;
    }
    
    if(bingo == false)
    {
        printf("Nothing found\n");
    }    
} //crack


//check if the hashes match            
bool psw_found( char* password, char* hashed)
{
    //in this problem "salt" is always 50
    char* cracked = crypt(password, "50");
    if (strcmp(cracked, hashed) == 0)
     {
        //print the password
        printf("%s\n", password);
        return true;
     }
    else
        return false;
}//psw_found
