/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define SIZE 728
//26 + 27*26 = 728

/**
 * Returns true if word is in dictionary else false.
 */
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

int hashfunction(const char *word);

int getIndex(const char c);

#endif // DICTIONARY_H
