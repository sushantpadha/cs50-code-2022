#include <math.h>
#include <stdbool.h>
#include "helpers.h"

void count_pixel(int *r, int *g, int *b, float *c, RGBTRIPLE px);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate over every row in image
    for (int i = 0; i < height; i++)
    {
        // iterate over every pixel in row
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE px = image[i][j];
            // get average of rgb values as nearest int
            int avg = round((px.rgbtRed + px.rgbtGreen + px.rgbtBlue) / 3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate over every row in image
    for (int i = 0; i < height; i++)
    {
        // iterate over every pixel in row
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE px = image[i][j];
            // convert to sepia using formulae
            int sepiaRed = round(.393 * px.rgbtRed + .769 * px.rgbtGreen + .189 * px.rgbtBlue);
            int sepiaGreen = round(.349 * px.rgbtRed + .686 * px.rgbtGreen + .168 * px.rgbtBlue);
            int sepiaBlue = round(.272 * px.rgbtRed + .534 * px.rgbtGreen + .131 * px.rgbtBlue);
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // half of the width rounded down to the nearest int
    int halfwidth = width / 2;
    // iterate over every row in image
    for (int i = 0; i < height; i++)
    {
        // iterate over every pixel in first half of row
        for (int j = 0; j < halfwidth; j++)
        {
            RGBTRIPLE px = image[i][j];
            // swap pixels
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = px;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create a copy of image, so that blurred pixels do not affect later calculations
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    // iterate over every row in image
    for (int i = 0; i < height; i++)
    {
        // iterate over every pixel in row
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE px = copy[i][j];

            // sum of all rgb values seen so far, and number of pixels counted
            int sumRed = px.rgbtRed;
            int sumBlue = px.rgbtBlue;
            int sumGreen = px.rgbtGreen;
            float numPx = 1.0;  // makes later division output float

            // note what edges this pixel touches
            bool topEdge = (i == 0);
            bool rightEdge = (j == width - 1);
            bool bottomEdge = (i == height - 1);
            bool leftEdge = (j == 0);

            // based on what edges are NOT touched, count different pixels
            // we are using *not* touching as criteria because
            // it is more general and reduces edge cases (pun intended)

            // * Check for NOT touching edges
            // add the pixel on the TOP
            if (!topEdge)
            {
                count_pixel(&sumRed, &sumGreen, &sumBlue, &numPx, copy[i - 1][j]);
            }
            // add the pixel on the RIGHT
            if (!rightEdge)
            {
                count_pixel(&sumRed, &sumGreen, &sumBlue, &numPx, copy[i][j + 1]);
            }
            // add the pixel on the BOTTOM
            if (!bottomEdge)
            {
                count_pixel(&sumRed, &sumGreen, &sumBlue, &numPx, copy[i + 1][j]);
            }
            // add the pixel on the LEFT
            if (!leftEdge)
            {
                count_pixel(&sumRed, &sumGreen, &sumBlue, &numPx, copy[i][j - 1]);
            }
            // * Check for NOT touching corners
            // add the pixel on the TOP LEFT
            if (!topEdge && !leftEdge)
            {
                count_pixel(&sumRed, &sumGreen, &sumBlue, &numPx, copy[i - 1][j - 1]);
            }
            // add the pixel on the TOP RIGHT
            if (!topEdge && !rightEdge)
            {
                count_pixel(&sumRed, &sumGreen, &sumBlue, &numPx, copy[i - 1][j + 1]);
                // add the pixel on the BOTTOM RIGHT
            }
            if (!bottomEdge && !rightEdge)
            {
                count_pixel(&sumRed, &sumGreen, &sumBlue, &numPx, copy[i + 1][j + 1]);
                // add the pixel on the BOTTOM LEFT
            }
            if (!bottomEdge && !leftEdge)
            {
                count_pixel(&sumRed, &sumGreen, &sumBlue, &numPx, copy[i + 1][j - 1]);
            }
            // calculate average by dividing by numPx
            image[i][j].rgbtRed = round(sumRed / numPx);
            image[i][j].rgbtGreen = round(sumGreen / numPx);
            image[i][j].rgbtBlue = round(sumBlue / numPx);
        }
    }
    return;
}

void count_pixel(int *r, int *g, int *b, float *c, RGBTRIPLE px)
{
    *r = *r + px.rgbtRed;
    *g = *g + px.rgbtGreen;
    *b = *b + px.rgbtBlue;
    *c = *c + 1;
    return;
}
