#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include "dictionary.h"

// maximum to characters to use for hashing
// 27^i for i > 5 will overflow on 64 bit systems
// mult that by 2 to account for adding smaller powers, and reduce i by one to account for mult by char code
// * using 3 for the best time space tradeoff (`./speller dictionaries/large texts/lalaland.txt` runs in 0.15s)
const unsigned int MAX_CHARS = 3;
const unsigned int TOTAL_CHARS = 27;
// type this manually to make it a compile time constant
// N = TOTAL_CHARS ^ (MAX_CHARS + 1)  (+1 accounts for mult by char code)
const unsigned int N = TOTAL_CHARS * TOTAL_CHARS * TOTAL_CHARS * TOTAL_CHARS;

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash all two char strings uniquely
    // uses MAX_CHARS characters for hashing
    // assumes that any char can be 'A'..'Z' or apostrophe
    // even though first char will never be apostrophe

    int h = 0;
    int wordlen = strlen(word);

    for (int n = 0; n < MAX_CHARS + 1; n++)
    {
        // If n is too big to index, break
        if (n > wordlen)
        {
            break;
        }
        // goes from 0..25, or -26 if char is apostrophe (or `\039`)
        int char_code = toupper(word[n]) - 'A';
        // wrap around negative value to TOTAL_CHARS-1 (or 26)
        char_code = (char_code < 0) ? TOTAL_CHARS - 1 : char_code;
        // multiply by a power of TOTAL_CHARS to make all values unique
        h += char_code * (int)pow(TOTAL_CHARS, MAX_CHARS - n);
    }

    return h;
}