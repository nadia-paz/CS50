#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define LENGTH 45
#define SIZE 728

int getIndex(const char c);
int hashfunction(const char *word);
bool load(const char *dictionary);
bool unload(void);
bool check(const char *word);


typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;
node *hashtable[SIZE];

int main(int argc, char *argv[])
{
   
    const char *dictionary;
    if (argc != 2)
    {
        printf("I need a word\n");
        return 1;
    }
    else
        dictionary = argv[1];
    
    load(dictionary);
  
    
    
    char *toprint;
    for(int i = 0; i < SIZE; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            toprint = cursor -> word;
            printf("word: %s hash: %i\n", toprint, hashfunction(toprint));
            cursor = cursor -> next;
        }
    }
    const char *st = "aLmOst";
    const char *str = "almost";
    bool is_checked = check(st);
    if (is_checked)
        printf("Checked Correct\n");
    else
        printf("Word is not in the dictionary\n");
    int strc = strcasecmp(st, str);
    if ( !strc  )
    {
        printf("STRC OK\n");
    }
    unload();
   
}

//returns the key for the hash table
int hashfunction(const char *word)
{
   if (strlen(word) == 1)
       return getIndex(word[0]);
   else
   {
       int index1 = getIndex(word[0]);
       int index2 = getIndex(word[1]);
       return 26 + 27 * index1 + index2;
   }
}

int getIndex(const char c)
{
     if (c == '\'')
        return 26;
     else
        return tolower(c) % (int) 'a';
}

bool load(const char *dictionary)
{
    // TODO
    char word[LENGTH];
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("LOAD FUNCTION ERROR");
        unload();
        return false;
    }
    
    while (fscanf(fp, "%s", word) != EOF)
    {
       int index = hashfunction(word);
       if (index >= SIZE)
       {
           unload();
           return false;
       }
       //if there no words in the linked list hashtable[index]
       if (hashtable[index] == NULL)
       {
            hashtable[index] = malloc(sizeof(node));
            if (hashtable[index] == NULL)
            {
                unload();
                return false;
            }
            strcpy(hashtable[index]->word, word);
        }
        else
        {
            //creat a new node
            node *new_node = malloc(sizeof(node));
            if (new_node == NULL)
            {
                unload();
                return false;
            }
            strcpy(new_node->word, word);
            //insert a node into the hashtable[index] linked list
            if (hashtable[index]->next == NULL)
                hashtable[index]->next = new_node;
            else
            {
                new_node->next = hashtable[index]->next;
                hashtable[index]->next = new_node;
            }
        }
    }
    return true;
}
 
bool check(const char *word)
{
    bool match = false;
    int index = hashfunction(word);
    printf("Our index is %i\n", index);
    node *cursor = hashtable[index];
    while (cursor != NULL)
    {
        printf("Word is %s\n", cursor -> word);
        if (!strcasecmp(cursor -> word, word))
        {
            printf("Cursor %s word %s\n", cursor -> word, word);
            printf("MATCH!!!!\n");
            match = true;
            break;
        }
        else    
            cursor = cursor -> next;
    }
    return match;
}

bool unload(void)
{
   // bool is_unloaded = false;
    for(int i = 0; i < SIZE; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor -> next;
            free(temp);
        }
    }
    
    return true;
}