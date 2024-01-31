#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

#define _XOPEN_SOURCE

const int MAX_PSW_LENGTH = 4;
const int ALPHABET_LENGTH = 5;

char curPassword[MAX_PSW_LENGTH+1];

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
    if ( hashed == NULL)
    {
        printf("Problem");
        return 1;
    }
    
    printf("%s\n", hashed);
    printf("Hash for ebd is %s\n", crypt("ebd", "50"));//50WvvtUlp10zQ
    crack(hashed);
    return 0;
}//main

void crack( char* hashed)
{
    char alphabet[ALPHABET_LENGTH];
    alphabet[0] = 'a';
    printf("%c", alphabet[0]);
    //fill the array with letters
       // for (int i = 1; i < 26; i++)
    for (int i = 1; i < ALPHABET_LENGTH; i++)
        {
            
            alphabet[i] = (char) alphabet[i-1] + 1;
            printf("%c", alphabet[i]);
        }
    printf("\n");
    
    int psw_length = 1;
    int index = 0;
    bool length_modified = false;
    bool bingo = false; //psw is cracked
    char* password;
    curPassword[MAX_PSW_LENGTH] = '\0';
    
    //check the psw 1 letter long
    for(int i = 0; i < ALPHABET_LENGTH; i++)
    {
        curPassword[index] = alphabet[i];
        
        password = curPassword;
        if(psw_found(password, hashed))
        {
            bingo = true;
            break;
        }
    }
    
    while(bingo == false || psw_length <= MAX_PSW_LENGTH)
    {
        for(int i = 0; i < ALPHABET_LENGTH; i++)
        {
        curPassword[index] = alphabet[i];
        password = curPassword;
        if(!length_modified)
        {
            curPassword[index+1] = 'a';
            psw_length++;
            length_modified = true;
        }
        else 
        {
            curPassword[index+1] += (char) sizeof(char);
            printf("curPassword[index+1] = %c\n", curPassword[index+1]);
        }
        password = curPassword;
        if(psw_found(password, hashed))
         {
            bingo = true;
            break;
         }
        length_modified = false;
        }
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
        printf("%s\n", password);
        return true;
     }
    else
        return false;
}//psw_found