#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// define a byte to be the same an unsigned 8 bit integer
typedef uint8_t BYTE;
// define number of bytes in a block
const int BLOCK_SIZE = 512;
// define starting 3 bytes of every JPEG file
const BYTE HEADER_START[3] = {0xff, 0xd8, 0xff};
const int NUM_HEADER_ENDS = 16;
const BYTE HEADER_END[NUM_HEADER_ENDS] = {0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef};
// define the digits in the filename
const int FILENAME_DIGITS = 3;

int main(int argc, char *argv[])
{
    // Check for proper command line usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open file for reading
    FILE *rawfile = fopen(argv[1], "r");
    if (rawfile == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // read block
    BYTE block[BLOCK_SIZE];

    // initialize loop variables that are needed between iterations
    int count = 0;
    bool is_jpgfile_open = false;
    FILE *jpgfile;

    // Keep reading 512-byte blocks
    while (fread(block, sizeof(BYTE), BLOCK_SIZE, rawfile) == BLOCK_SIZE)
    {
        // Check the header's first 3 bytes (start) and 4th byte (end)
        bool is_header_start_valid = ((block[0] == HEADER_START[0]) &&
                                      (block[1] == HEADER_START[1]) &&
                                      (block[2] == HEADER_START[2]));
        bool is_header_end_valid = false;
        for (int i = 0; i < NUM_HEADER_ENDS; i++)
        {
            if (block[3] == HEADER_END[i])
            {
                is_header_end_valid = true;
                break;
            }
        }

        // New file has started
        if (is_header_start_valid && is_header_end_valid)
        {
            if (is_jpgfile_open)
            {
                fclose(jpgfile);
            }
            is_jpgfile_open = true;

            // Open a new file for writing; close last file and redefine pointer
            char *jpgfilename = malloc(sizeof(char) * (FILENAME_DIGITS + 4 + 1));
            if (jpgfilename == NULL)
            {
                return 1;
            }
            sprintf(jpgfilename, "%03i.jpg", count);

            jpgfile = fopen(jpgfilename, "w");

            // Write to file
            fwrite(block, sizeof(BYTE), BLOCK_SIZE, jpgfile);

            // Increment file count
            count++;

            free(jpgfilename);
        }
        // Still reading from the last opened jpg file
        else
        {
            if (is_jpgfile_open)
            {
                fwrite(block, sizeof(BYTE), BLOCK_SIZE, jpgfile);
            }
        }
    }

    // Close file
    fclose(rawfile);

    return 0;
}
