#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int valid_key(string key);

int main(int argc, string argv[])
{
    // Checks the number of arguments in the input
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Validates the key
    if (valid_key(argv[1]))
    {
        return 1;
    }

    // Asks for user's input
    string plain_text = get_string("plaintext: ");
    int length_text = strlen(plain_text);
    char cipher_text[length_text];

    for (int i = 0; i < length_text; i++)
    {
        if (isalpha(plain_text[i]))
        {
            if (isupper(plain_text[i]))
            {
                cipher_text[i] = toupper(argv[1][plain_text[i] - 'A']);
            }
            else if (islower(plain_text[i]))
            {
                cipher_text[i] = tolower(argv[1][plain_text[i] - 'a']);
            }
        }
        else
        {
            cipher_text[i] = plain_text[i];
        }
    }
    // add end of line
    cipher_text[length_text] = '\0';

    printf("ciphertext: %s", cipher_text);
    printf("\n");
    return 0;
}

int valid_key(string key)
{
    // Checks if the key has 26 letters
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;

    }
    int aux[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // Checks if the key only contain alphabetic characters.
    for (int i = 0; i < 26; i++)
    {
        if (isdigit(key[i]))
        {
            printf("Key must contain only alphabetic characters.\n");
            return 1;
        }
        aux[tolower(key[i]) - 'a'] = 1;
    }

    // Checks if the key contains repeated elements
    int total = 0;
    for (int i = 0; i < 26; i++)
    {
        if (aux[i] == 1)
        {
            total++;
        }
    }

    if (total != 26)
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }
    return 0;
}