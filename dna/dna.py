import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Read database file into a variable
    database = {}
    with open(sys.argv[1], "r") as file1:
        reader = csv.DictReader(file1)
        database = list(reader)

    # Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file2:
        sequence = file2.read()

    # Find longest match of each STR in DNA sequence and add it to a dictionary
    matches = dict()
    for subsequence in database[0]:
        longest_run = longest_match(sequence, subsequence)
        matches[subsequence] = longest_run
    # Delete key 'name' from the dictionary
    del matches['name']

    # Check database for matching profiles
    count = 0
    no_match = 1
    for i in range(len(database)):
        for j in matches:
            if str(matches[j]) == database[i][j]:
                count += 1

        # Check if it matches the person
        if count == len(matches):
            print(database[i]['name'])
            no_match = 0
            break
        else:
            count = 0

    if no_match:
        print("No match")
        
    sys.exit(0)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each charer in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
