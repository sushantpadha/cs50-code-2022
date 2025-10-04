from cs50 import get_string


def main():
    # get input text
    text = get_string("Text: ")
    # get number of tokens (letters, words, sentences)
    letters, words, sentences = count_tokens(text)
    # calculate coleman-liau index
    index = round(coleman_liau(letters, words, sentences))
    # print different outputs based on index value
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


def count_tokens(text):
    # initialize counts
    # number of letters = 0
    # number of words = 1 (the first word already exists)
    # number of words = 0
    letters, words, sentences = 0, 1, 0

    for char in text:
        # if char is letter
        if char.isalpha():
            letters += 1
        # if char is word terminator
        elif char == " ":
            words += 1
        # if char is sentence terminator
        elif char in ".!?":
            sentences += 1

    return letters, words, sentences


def coleman_liau(letters, words, sentences):
    # letters per 100 words
    L = letters / words * 100
    # sentences per 100 words
    S = sentences / words * 100
    # calculate index
    index = 0.0588 * L - 0.296 * S - 15.8
    return index


if __name__ == "__main__":
    main()
