import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")
    data_csv, sequence_txt = sys.argv[1:3]

    # Read database file into a variable
    data = []
    with open(data_csv) as f:
        reader = csv.DictReader(f)
        for row in reader:
            name = row["name"]
            # mapping of names to a (mapping of STRS to their frequency)
            profile = {name: {}}
            profile[name] = {k: int(row[k]) for k in row if k != "name"}
            data.append(profile)

    # Read DNA sequence file into a variable
    with open(sequence_txt) as f:
        sequence = f.read()

    # Find longest match of each STR in DNA sequence
    occurences = {}
    for profile in data:
        # get name (key) and sequences dict (value)
        name, sequences = tuple(profile.items())[0]
        # find the occurence of every sequence
        for seq in sequences:
            occurence = longest_match(sequence, seq)
            occurences[seq] = occurence

    # Check database for matching profiles
    match = None
    # iterate over every database profile
    for profile in data:
        invalid = False
        name, sequences = tuple(profile.items())[0]
        # iterate over every sequence in profile
        for seq, freq in sequences.items():
            # if the sequence does not match, set invalid to true and break
            if occurences[seq] != freq:
                invalid = True
                break
        # if this entry is invalid, skip to next one
        if invalid:
            continue
        # this entry is valid, set match and break
        match = name
        break

    if match is None:
        print("No match.")
    else:
        print(match)

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
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


if __name__ == "__main__":
    main()
