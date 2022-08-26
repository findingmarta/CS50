// Implements a dictionary's functionality

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table - odd number closer to the 2*number of words in the input dictionary
const unsigned int N = 143091 / 2;

// Hash table
node *table[N];

// Number of words in dictionary
unsigned int total = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *cursor = table[index];

    // Start parsing the list
    while (cursor != NULL)
    {
        // Compare both words
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }

        // If the words don't match then moves on to the next element in the list
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number between 0 and N-1, inclusive
unsigned int hash(const char *word)
{
    // odd number to generate more unique numbers;31 is a special number because, according to the Data Structures & Algorithms in Java, has fewer collisions.
    int constant = 31;
    int hash = 0;
    int c = 0;             // to parse the word

    while (c == *word++)
    {
        hash = constant * hash + word[c];
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Couldn't open the dictionary file!\n");
        return false;
    }

    // Read strings from the file one at a time
    char word [LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Couldn't create a new node!\n");
            return false;
        }
        strcpy(n->word, word);
        n->next = NULL;

        // Hash word to obtain a hash value
        int index = hash(word);

        // Insert node into hash table at that location
        n->next = table[index];
        table[index] = n;
        total++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int index = 0; index < N; index++)
    {
        while (table[index] != NULL)
        {
            node *tmp = table[index]->next;
            free(table[index]);
            table[index] = tmp;
        }
    }

    return true;
}
