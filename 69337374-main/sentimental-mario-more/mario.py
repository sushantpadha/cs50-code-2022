from cs50 import get_int


def main():
    height = 0
    # get height from user
    while (height < 1 or height > 8):
        height = get_int("Height: ")

    for bricks in range(1, height + 1):
        # number of spaces to print
        spaces = height - bricks
        print(" " * spaces, end="")    # print spaces
        print("#" * bricks, end="  ")  # print bricks
        print("#" * bricks, end="\n")    # print bricks


if __name__ == "__main__":
    main()
