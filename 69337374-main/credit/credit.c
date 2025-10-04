#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

string number_category(long num);
bool check_first_digits(long num, long digits);
long number_of_digits(long num);
bool luhns_algorithm(long num);

int main(void)
{
    // get the number
    long number = get_long("Number: ");

    // get the number's category
    string category = number_category(number);

    // print the output
    printf("%s\n", category);
}

string number_category(long num)
{
    // AMEX:       len = 15     ; start = 34, 37
    // MASTERCARD: len = 16     ; start = 51, 52, 53, 54, 55
    // VISA:       len = 13, 16 ; start = 4
    string category = "INVALID";
    long digits = number_of_digits(num);

    switch (digits)
    {
        case 15:
        {
            // array of all possible first digits
            long first_digits_arr[2] = {34, 37};
            // try all possible first digits
            for (int i = 0; i < 2; i++){
                if (check_first_digits(num, first_digits_arr[i]))
                {
                    // if one works, change category and break
                    category = "AMEX";
                    break;
                }
            }
            break;
        }
        case 13:
        {
            long first_digits_arr[1] = {4};
            for (int i = 0; i < 1; i++){
                if (check_first_digits(num, first_digits_arr[i]))
                {
                    category = "VISA";
                    break;
                }
            }
            break;
        }
        case 16:
        {
            long first_digits_arr_1[5] = {51, 52, 53, 54, 55};
            for (int i = 0; i < 5; i++){
                if (check_first_digits(num, first_digits_arr_1[i]))
                {
                    category = "MASTERCARD";
                    break;
                }
            }

            long first_digits_arr_2[1] = {4};
            for (int i = 0; i < 1; i++){
                if (check_first_digits(num, first_digits_arr_2[i]))
                {
                    category = "VISA";
                    break;
                }
            }
            break;
        }
    }

    // if category is still INVALID, return it
    if (strcmp(category, "INVALID") == 0)
    {
        return category;
    }

    // if we are executing here, the card atleast belongs a category
    // now find if it is valid or not
    bool valid = luhns_algorithm(num);
    // if not valid, set category to invalid
    if (!valid)
    {
        category = "INVALID";
    }
    return category;
}

bool luhns_algorithm(long num)
{
    // output of the calculation
    long output = 0;

    // number of digits in num
    long num_digits = number_of_digits(num);
    // output of doubling index i and adding its digits
    int double_and_add_digits[10] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};
    // define variables to be redefined in the loop
    long digit_value;
    int ith_digit;

    // iterate over every digit
    for (int i = 0; i < num_digits; i++)
    {
        // the value represented by the digit, i.e., its "place value"
        digit_value = (long) pow(10, i);
        // get the ith digit by doing:
        // num mod 10^(i+1): which gives all the digits less than i
        // (integer) divided by 10^i: which gives the first digit from this
        ith_digit = (num % (10 * digit_value)) / digit_value;
        // if we are dealing with an odd numbered digit
        if (i % 2 == 1)
        {
            // take the digit, double it, and add its constituent digits
            // for digits less than 5, its just the double of the digit
            // for digits gt eq to 5, its the same, plus 1
            if (ith_digit >= 5)
            {
                output += 1;
                ith_digit %= 5;
            }
            output += 2 * ith_digit;
        }
        // if we are dealing with an even numbered digit, add the digit
        else
        {
            output += ith_digit;
        }
    }
    // if output is a multiple of 10, return true, else false
    if (output % 10 == 0)
    {
        return true;
    }

    return false;
}

bool check_first_digits(long num, long first)
{
    // number of digits in num
    long num_digits = number_of_digits(num);
    // number of digits in first
    long first_digits = number_of_digits(first);
    // find the highest power which should not change
    // eg: for num=2154, first=21; digits upto 10^(4-2) must be constant, i.e., 54 should not change
    long most_sig_digit = (long) pow(10, num_digits - first_digits);
    // the value represented by first, i.e., its "place value"
    // same eg: first actually represents a "place value" of 21000
    long first_value = first * most_sig_digit;
    // the remainder when num is divided by most_sig_digit
    long remainder_digits = num % most_sig_digit;
    // now, the difference between num and the value of digits
    // should be the same as the remainder
    if (num - first_value == remainder_digits)
    {
        return true;
    }
    return false;
}

long number_of_digits(long num)
{
    // copy of num
    long _num = num;
    // number of digits
    long numdigits = 0;
    while (_num != 0)
    {
        _num /= 10;  // integer division
        numdigits++;
    }
    return numdigits;
}
