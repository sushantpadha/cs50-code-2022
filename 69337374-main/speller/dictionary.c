// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

const unsigned int TOTAL_CHARS = 27;
const unsigned int N = TOTAL_CHARS * TOTAL_CHARS * TOTAL_CHARS * TOTAL_CHARS;

// Hash table
node *table[N];

void init_table(void);
bool append(node *root, char word[]);
void print_table(void);
void print_node(node *root);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get hash of word
    int h = hash(word);
    // Iterate over every node from table[h] until we find word
    node *n = table[h];
    while (n != NULL)
    {
        // check for word
        if (strcasecmp(n->word, word) == 0)
        {
            return true;
        }
        // update n
        n = n->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash all two char strings uniquely
    // uses MAX_CHARS characters for hashing
    // assumes that any char can be 'A'..'Z' or apostrophe
    // even though first char will never be apostrophe

    int h = 0;
    int wordlen = strlen(word);

    for (int i = 0; i < wordlen - 1; i++)
    {
        h += toupper(word[i]);
    }

    return h % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Initialize hash table
    init_table();

    char word[LENGTH + 1]; // word that we are reading
    int index = 0;         // index of the char in word
    char c;                // single character to read
    // Read file char wise
    while (fread(&c, sizeof(char), 1, file) == 1)
    {
        // If char is alphabet or apostrophe
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            word[index] = c;
            index++;
            // If char is longer than LENGTH
            if (index > LENGTH)
            {
                while (fread(&c, sizeof(char), 1, file) && isalpha(c))
                    ;
                index = 0;
            }
        }

        // If char is digit
        else if (isdigit(c))
        {
            while (fread(&c, sizeof(char), 1, file) && isalnum(c))
                ;
            index = 0;
        }

        // Else, we must have reached the end of word
        else if (index > 0)
        {
            word[index] = '\0';

            // Add word to dictionary
            // get hash
            int h = hash(word);
            // add word to node (linked list) at index h in hash table
            bool appended_word = append(table[h], word);
            if (!appended_word)
            {
                return false;
            }
            index = 0;
        }
    }

    // Close file
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Number of words in dictionary
    int words = 0;
    for (int i = 0; i < N; i++)
    {
        node *n = table[i]->next; // start from root->next
        while (n != NULL)
        {
            words++;
            n = n->next;
        }
    }
    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // declare a tmp node and current node
        node *tmp, *n;
        n = table[i];
        while (n != NULL)
        {
            // use tmp to store current node's next and
            // reset current node to tmp for next iteration
            // frees all elements iteratively
            tmp = n->next;
            free(n);
            n = tmp;
        }
    }
    return true;
}

// Initialize hash table value
void init_table(void)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        // initialize word value (there are probably better ways to do this)
        table[i]->word[0] = '#';
        table[i]->word[1] = '\0';
        table[i]->next = NULL;
    }
}

// Appends string to node and returns success as bool
bool append(node *root, char word[])
{
    // create new ptr to node
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        return false;
    }
    // change its word to word and its next ptr to root's next ptr
    strcpy(n->word, word);
    n->next = root->next;
    // change root's next ptr to n
    root->next = n;
    return true;
}

// Pretty print table
void print_table(void)
{
    for (int i = 0; i < N; i++)
    {
        printf("%i", i);
        print_node(table[i]);
        printf("\n");
    }
}

// Pretty print node
void print_node(node *root)
{
    printf("%s", root->word);
    printf("-->");
    if (root->next != NULL)
    {
        print_node(root->next);
    }
    else
    {
        printf("NULL");
    }
}
