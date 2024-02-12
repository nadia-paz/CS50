/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

node *hashtable[SIZE];
int count = 0;

int getIndex(const char c)
{
     if (c == '\'')
        return 26;
     else
        return tolower(c) % (int) 'a';
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
/**
 * Returns true if word is in dictionary else false.
 */
 
bool check(const char *word)
{
    bool match = false;
    int index = hashfunction(word);
    node *cursor = hashtable[index];
    while (cursor != NULL)
    {
        //when returns 0 if is false
        if (!strcasecmp(cursor -> word, word))
        {
            match = true;
            break;
        }
        else    
            cursor = cursor -> next;
    }
    return match;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
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
        count++;
    }//while
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
   return count;;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
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