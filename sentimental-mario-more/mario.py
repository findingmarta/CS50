def main():
    height = get_height()

    for i in range(height):
        # print spaces
        for j in range(height - i - 1):
            print(" ", end="")

        # print hashtags
        for j in range(i + 1):
            print("#", end="")

        print("  ", end="")

        # print last hashtags
        for j in range(i + 1):
            print("#", end="")

        print()


def get_height():
    while True:
        try:
            n = int(input("Height: "))
            if n >= 1 and n <= 8:
                break
        except ValueError:
            print("That's not an integer!")
    return n


main()