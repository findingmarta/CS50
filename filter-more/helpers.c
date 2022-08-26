#include "helpers.h"
#include <math.h>

typedef struct
{
    LONG  rgbtBlue;
    LONG  rgbtGreen;
    LONG  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE_TEMP;

void swap(RGBTRIPLE *a, RGBTRIPLE *b);
void init_temp(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE_TEMP temp[height][width]);
void update_image(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE_TEMP temp[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE value = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float) 3);
            image[i][j].rgbtBlue = value;
            image[i][j].rgbtGreen = value;
            image[i][j].rgbtRed = value;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int start = 0;
        int end = width - 1;
        while (start < end)
        {
            swap(&image[i][start], &image[i][end]);
            start++;
            end--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Temporary array to store pixels inorder to avoid overwriting any
    RGBTRIPLE_TEMP temp[height][width];

    // Initialize
    init_temp(height, width, image, temp);

    // Loop over the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize all variables to 0
            int total = 0;
            int blue = 0;
            int green = 0;
            int red = 0;

            // For each pixel we will chech over down, up, left and right to see if it meets the condition
            for (int row = -1; row <= 1; row++)
            {
                for (int col = -1; col <= 1; col++)
                {
                    // Check if it is still inside the image
                    if (i + row > height - 1 || i + row < 0 || j + col > width - 1 || j + col < 0)
                    {
                        continue;
                    }

                    // If not the we must loop and add pixels at all corners of our current pixel
                    blue += image[i + row][j + col].rgbtBlue;
                    green += image[i + row][j + col].rgbtGreen;
                    red += image[i + row][j + col].rgbtRed;
                    total++;
                }
            }

            // Register the new pixel
            temp[i][j].rgbtBlue = round((float) blue / (float) total);
            temp[i][j].rgbtGreen = round((float) green / (float) total);
            temp[i][j].rgbtRed = round((float) red / (float) total);
        }
    }

    // Update the image
    update_image(height, width, image, temp);
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Temporary array to store pixels inorder to avoid overwriting any
    RGBTRIPLE_TEMP temp[height][width];

    // Initialize
    init_temp(height, width, image, temp);

    // Kernel in the x direction
    float kernelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    // Kernel in the y direction
    float kernelY[3][3] = {{-1, -2, -1}, { 0,  0,  0}, { 1,  2,  1}};

    int blueX, greenX, redX, blueY, greenY, redY;

    // Loop over the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize all variables to 0
            blueX = greenX = redX = blueY = greenY = redY = 0;

            // For each pixel we will chech over down, up, left and right to see if it meets the condition
            for (int row = -1; row <= 1; row++)
            {
                for (int col = -1; col <= 1; col++)
                {
                    // Check if it is still inside the image
                    if (i + row > height - 1 || i + row < 0 || j + col > width - 1 || j + col < 0)
                    {
                        continue;
                    }

                    // Calculate the new value
                    blueX += kernelX[row + 1][col + 1] * image[i + row][j + col].rgbtBlue;
                    greenX += kernelX[row + 1][col + 1] * image[i + row][j + col].rgbtGreen;
                    redX += kernelX[row + 1][col + 1] * image[i + row][j + col].rgbtRed;

                    blueY += kernelY[row + 1][col + 1] * image[i + row][j + col].rgbtBlue;
                    greenY += kernelY[row + 1][col + 1] * image[i + row][j + col].rgbtGreen;
                    redY += kernelY[row + 1][col + 1] * image[i + row][j + col].rgbtRed;
                }
            }

            // Register the new pixel and cap value if necessary
            temp[i][j].rgbtBlue = round(sqrt(blueX * blueX + blueY * blueY));
            if (temp[i][j].rgbtBlue > 255)
            {
                temp[i][j].rgbtBlue = 255;
            }

            temp[i][j].rgbtGreen = round(sqrt(greenX * greenX + greenY * greenY));
            if (temp[i][j].rgbtGreen > 255)
            {
                temp[i][j].rgbtGreen = 255;
            }

            temp[i][j].rgbtRed = round(sqrt(redX * redX + redY * redY));
            if (temp[i][j].rgbtRed > 255)
            {
                temp[i][j].rgbtRed = 255;
            }
        }
    }

    // Update the image
    update_image(height, width, image, temp);
}

// Inicializes the temporay array with the original info of the image
void init_temp(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE_TEMP temp[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j].rgbtBlue = image[i][j].rgbtBlue;
            temp[i][j].rgbtGreen = image[i][j].rgbtGreen;
            temp[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }
    return;
}

// Updates the image using a temporary array
void update_image(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE_TEMP temp[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
    return;
}

// Swaps two RGBTRIPLEs
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp = *a;
    *a = *b;
    *b = temp;
    return;
}

