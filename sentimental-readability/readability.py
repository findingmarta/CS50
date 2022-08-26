import re


def main():
    # Asks for input
    text = input("Text: ")

    # Starts parsing the text
    num_letters = count_letters(text)
    num_words = count_words(text)
    num_sentences = count_sentences(text)

    # Aplies the formula
    L = num_letters / num_words * 100
    S = num_sentences / num_words * 100
    index = 0.0588 * L - 0.296 * S - 15.8

    # Decides the output
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index:.0f}")


# Counts the number of letters in the text
def count_letters(text):
    letters = re.findall("\w", text)
    letters = list(filter(("_").__ne__, letters))
    return len(letters)


# Counts the number of words in the text
def count_words(text):
    words = re.split("\s", text)
    return len(words)


# Counts the number of sentences in the text
def count_sentences(text):
    sentences = re.split("[.!?]", text)
    return len(sentences) - 1


if __name__ == "__main__":
    main()