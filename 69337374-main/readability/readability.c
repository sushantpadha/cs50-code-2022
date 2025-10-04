#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
double coleman_liau(int letters, int words, int sentences);

int main(void)
{
    // get input text
    string text = get_string("Text: ");
    // number of letters
    int letters = count_letters(text);
    // number of words
    int words = count_words(text);
    // number of letters
    int sentences = count_sentences(text);
    // calculate coleman-liau index
    int index = round(coleman_liau(letters, words, sentences));
    // print different outputs based on index value
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
    // declare count to be 0
    int count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        // if char is alphabetic, increment count
        if (isalpha(text[i]) != 0)
        {
            count++;
        }
    }
    return count;
}

int count_words(string text)
{
    // declare count to be 1
    // because words exist in between spaces
    // and one word exists between start of string and the first space
    int count = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        // if char is a space, increment count
        if (text[i] == ' ')
        {
            count++;
        }
    }
    return count;
}

int count_sentences(string text)
{
    // declare count to be 0
    int count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        char c = text[i];
        // if char is one of .!?, increment count
        if ((c == '.') || (c == '!') || (c == '?'))
        {
            count++;
        }
    }
    return count;
}

double coleman_liau(int letters, int words, int sentences)
{
    // cast `words` to double
    double _words = (double) words;
    // now all divisions will return doubles
    double L = letters / _words * 100;
    double S = sentences / _words * 100;
    double index = 0.0588 * L - 0.296 * S - 15.8;
    return index;
}