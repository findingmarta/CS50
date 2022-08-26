#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    char filename[32];
    BYTE buf[512];
    int bytes, jpg_found = 0, name = 0;
    FILE *output = NULL;

    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Starts reading the file
    while ((bytes = (fread(buf, sizeof(BYTE), 512, file))) > 0)
    {
        // Check first four bytes
        if (buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff && ((buf[3] & 0xf0) == 0xe0))
        {
            if (jpg_found == 1)
            {
                fclose(output);
            }
            else
            {
                jpg_found = 1;
            }
            // Creates a new file to save the image
            sprintf(filename, "%03i.jpg", name++);

            // Opens file
            output = fopen(filename, "w");
            if (output == NULL)
            {
                printf("Could not open output file.\n");
                return 1;
            }
        }

        if (jpg_found == 1)
        {
            // Writes the information in the new file
            fwrite(&buf, sizeof(BYTE), bytes, output);
        }
    }

    // Close file
    fclose(output);
    fclose(file);
    return 0;
}



