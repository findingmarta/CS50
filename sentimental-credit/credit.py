from cs50 import get_int
import re


def main():
    while True:
        card_number = get_int("What's your credit card number?\n")
        if card_number > 0:
            break

    # Convert to string and get the total number of digits in the string
    num_digits = len(str(card_number))

    # Checks if the card number is valid
    valid = check_card(card_number, num_digits)
    if valid:
        # If so we try to get the type of the card
        get_card(str(card_number), num_digits)
    else:
        print("INVALID")


def check_card(card_number, num_digits):
    numbers = list()
    divisor = 1
    sum = 0

    # Stores digits into a list in reverse order
    for i in range(num_digits):
        numbers.append((card_number // divisor) % 10)
        divisor = divisor * 10

    for i in range(1, num_digits, 2):
        # 2 cases: product < 10 and >=10
        if numbers[i] * 2 < 10:
            sum = sum + numbers[i] * 2
        else:
            sum = sum + (numbers[i] * 2) % 10 + ((numbers[i] * 2) // 10) % 10

    for i in range(0, num_digits, 2):
        sum = sum + numbers[i]

    if (sum % 10) != 0:
        return 0

    return 1


def get_card(card_number, num_digits):
    if num_digits == 15 and re.search("^3", card_number) and re.search("4|7", card_number[1]):
        print("AMEX")
    elif num_digits == 16 and re.search("^5", card_number) and re.search("[12345]", card_number[1]):
        print("MASTERCARD")
    elif num_digits >= 13 and num_digits <= 16 and re.search("^4", card_number):
        print("VISA")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()