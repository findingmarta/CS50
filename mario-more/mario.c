#include <cs50.h>
#include <stdio.h>

void printLine(char c, int height, int i);

int main(void)
{
    int height;
    // checks for valid input
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    //for each row
    for (int i = 0; i < height; i++)
    {
        printLine(' ', height, i);
        printLine('#', height, i);
        printf(" ");

        printf(" ");
        printLine('#', height, i);
        printf("\n");
    }
}








// prints a line
void printLine(char c, int height, int i)
{
    if (c == ' ')
    {
        for (int j = 1; j < height - i; j++)
        {
            printf(" ");
        }
    }

    if (c == '#')
    {
        for (int j = height - i; j <= height; j++)
        {
            printf("#");
        }
    }
}
