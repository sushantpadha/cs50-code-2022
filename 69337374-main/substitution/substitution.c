#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

string substitution_cipher(string plaintext, string key);
int validate_key(string key);

int main(int argc, string argv[])
{
    // check number of arguments
    // if not 2, return with error
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // get key
    string key = argv[1];
    int key_status = validate_key(key);
    // based on key status print out different error messages
    switch (key_status)
    {
        case 1:
        {
            printf("Key must contain 26 characters.\n");
            break;
        }
        case 2:
        {
            printf("Key must contain alphabetic characters only.\n");
            break;
        }
        case 3:
        {
            printf("Key must contain unique alphabetic characters only.\n");
            break;
        }
    }
    // return 1 if key status is non-zero
    if (key_status != 0)
    {
        return 1;
    }

    // ask for plaintext
    string plaintext = get_string("plaintext: ");

    // get cipher text
    string ciphertext = substitution_cipher(plaintext, key);

    // print cipher text
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

string substitution_cipher(string plaintext, string key)
{
    // note: modifies plaintext in-place
    // iterate over every character in plaintext
    for (int i = 0; i < strlen(plaintext); i++)
    {
        char c = plaintext[i];
        // if c is non alphabetic, skip
        if (isalpha(c) == 0)
        {
            continue;
        }
        // get the position of this letter in the english alphabet
        int letter_i = (int) c;
        int ascii_offset, c_is_lowercase;
        if (islower(c))
        {
            c_is_lowercase = 1;
            // lowercase letters start from 97
            ascii_offset = 97;
        }
        else
        {
            c_is_lowercase = 0;
            // uppercase letters start from 65
            ascii_offset = 65;
        }

        // subtract an offset to the letter's actual index
        letter_i -= ascii_offset;

        // get new or encrypted character from key
        char new_c = key[letter_i];
        // set new_c to upper or lower based on case of c
        if (c_is_lowercase)
        {
            new_c = tolower(new_c);
        }
        else
        {
            new_c = toupper(new_c);
        }

        // set char in plaintext to new_c
        plaintext[i] = new_c;
    }
    return plaintext;
}

int validate_key(string key)
{
    // error codes:
    // 0: all good (no error)
    // 1: key length is invalid
    // 2: key contains invalid characters
    // 3: key contains duplicate characters
    int keylen = (int) strlen(key);

    // check if key length is not 26
    if (keylen != 26)
    {
        return 1;
    }

    // check if key contains invalid chars
    for (int i = 0; i < keylen; i++)
    {
        if (!(isalpha(key[i])))
        {
            return 2;
        }
    }

    // check if key contains duplicate alphabets
    for (int i = 0; i < keylen; i++)
    {
        for (int j = 0; j < keylen; j++)
        {
            // dont check that the same character is equal to itself
            if (i == j)
            {
                continue;
            }

            // check if another character matches current one
            if (key[i] == key[j])
            {
                return 3;
            }
        }
    }

    return 0;
}
