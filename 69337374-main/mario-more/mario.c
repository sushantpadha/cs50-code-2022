#include <stdio.h>
#include <cs50.h>

void pyramid_row(int hashes, int spaces)
{
    for (int j = 0; j < spaces; j++)
    {
        printf(" ");
    }
    for (int j = 0; j < hashes; j++)
    {
        printf("#");
    }
}

void pyramid(int height)
{
    // maximum length, i.e., length of last row
    const int maxlength = height;
    for (int length = 0; length < height; length++)
    {
        // loop over all i from 0 to h-1
        // run `print_pyramid_line` with i + 1
        pyramid_row(length + 1, maxlength - length - 1);
        pyramid_row(length + 1, 2);
        printf("\n");
    }
}

int main(void)
{
    // define height to be 0 currently
    int height = 0;

    // ask for height until the input is correct
    while ((height <= 0) || (height > 8))
    {
        height = get_int("Height: ");
    }

    // call `print_pyramid`
    pyramid(height);
}
