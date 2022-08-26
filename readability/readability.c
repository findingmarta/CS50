#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // waits for input
    string text = get_string("Text: ");

    // starts the parsing of the text
    int num_letters = count_letters(text);
    int num_words = count_words(text);
    int num_sentences = count_sentences(text);

    // aplies the formula
    float L = (float) num_letters / (float) num_words * 100;
    float S = (float) num_sentences / (float) num_words * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // decides the output based on the result of the formula
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
    return 0;
}

int count_letters(string text)
{
    int length = strlen(text);
    int i = 0, letters = 0;

    // starts counting the number of letters/numbers
    while (i < length)
    {
        if (isalnum(text[i]))
        {
            letters++;
        }
        i++;
    }
    return letters;
}

int count_words(string text)
{
    int length = strlen(text);
    int i = 0, words = 0;

    // starts counting the number of words (separated by spaces)
    while (i < length)
    {
        if (isspace(text[i]))
        {
            words++;
        }
        i++;
    }
    return words + 1;
}

int count_sentences(string text)
{
    int length = strlen(text);
    int i = 0, sentences = 0;

    // starts counting the number of sentences (separated by '.', '!' or '?')
    while (i < length)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
        i++;
    }
    return sentences;
}