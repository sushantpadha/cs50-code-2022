from cs50 import get_string


def main():
    # get card number as string
    card_str = get_string("Number: ").rstrip()

    # get card category
    category = card_category(card_str)

    # print card category
    print(category)


def card_category(card):
    # AMEX:       len = 15     ; start = 34, 37
    # MASTERCARD: len = 16     ; start = 51, 52, 53, 54, 55
    # VISA:       len = 13, 16 ; start = 4

    # get length of card number
    card_len = len(card)
    category = "INVALID"

    # check for correct card length and starting digits
    if ((card_len == 15) and
            (card[:2] in ("34", "37"))):
        category = "AMEX"

    elif ((card_len == 16) and
            (card[:2] in ("51", "52", "53", "54", "55"))):
        category = "MASTERCARD"

    elif ((card_len in (13, 16)) and
            (card[0] == "4")):
        category = "VISA"

    # if card is still invalid, return
    if category == "INVALID":
        return category

    # check if card is valid (using luhns algorithm)
    valid = luhns_algorithm(card)
    if (not valid):
        category = "INVALID"

    return category


def luhns_algorithm(card):
    # init total to 0
    total = 0
    # get card len
    card_len = len(card)
    # iterate over all digit
    for i in range(card_len):
        # init value of this digit to 0
        value = 0
        # if digit is second to last (or an even no away from it)
        if (card_len - i) % 2 == 0:
            # get doubled digit as int
            doubled_digit = int(card[i]) * 2
            # add up the individual digit of doubled digit
            value += doubled_digit % 10   # last digit
            value += doubled_digit // 10  # first digit
        else:
            # add digit as int
            value += int(card[i])

    # return true if total ends in 0
    return (total // 10 == total % 10)


if __name__ == "__main__":
    main()
