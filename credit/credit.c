#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int checkCard(long n, int tam)
{
    int r, arr[tam], nums [tam], sum = 0, j = 0;
    for (int i = tam - 1; n != 0; i--)
    {
        r = n % 10;
        arr[i] = r;
        n = n / 10;
    }

    for (int i = tam - 2; i >= 0; i = i - 2)
    {
        nums[j++] = arr[i] * 2;
    }

    for (int i = 0; i < tam / 2; i++)
    {
        if (nums[i] <= 9)
        {
            sum += nums[i];
        }
        else
        {
            sum += nums[i] % 10;
            nums[i] = nums[i] / 10;
            sum += nums[i] % 10;
        }
    }

    for (int i = 1; i <= tam; i = i + 2)
    {
        sum += arr[tam - i];
    }

    sum = sum % 10;
    if (sum != 0)
    {
        return 0;
    }
    return 1;
}


int digits_only(const char *s)
{
    while (*s)
    {
        if (isdigit(*s++) == 0)
        {
            return 0;
        }
    }

    return 1;
}


int main(void)
{
    string st;
    do
    {
        st = get_string("What's your credit card number?\n");
    }
    while (!digits_only(st));

    int num_digits = strlen(st);
    int valid = checkCard(atoll(st), num_digits);

    if (num_digits == 15 && st[0] == '3' && (st[1] == '4' || st[1] == '7') && valid)
    {
        printf("AMEX\n");
    }
    else if (num_digits == 16 && st[0] == '5' && (st[1] == '1' || st[1] == '2' || st[1] == '3' || st[1] == '4' || st[1] == '5')
             && valid)
    {
        printf("MASTERCARD\n");
    }
    else if (num_digits >= 13 && num_digits <= 16 && st[0] == '4' && valid)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
