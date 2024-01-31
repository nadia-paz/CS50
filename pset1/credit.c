#include <stdio.h>
#include <cs50.h>
#include <math.h>

int digits_number(long long cc_number);
long long longpow(int a, int b);
int number_on_index(long long cc_number, int index);
int checksum(long long cc_number);
void validation(long long cc_number);

int main(void)
{
  printf("Number: ");
  long long cc_number = get_long_long();
  validation(cc_number);
}

//returns the number of digits of credit card 13, 15, 16
//returns -1 is the number of digits is not valid
int digits_number(long long cc_number)
 {
   
    if (cc_number >= longpow(10,12) && cc_number < (longpow(10, 13)-1))
        return 13;
    else if (cc_number >= longpow(10, 14) && cc_number < (longpow(10, 15)-1))
        return 15;
    else if (cc_number >= longpow(10, 15) && cc_number < (longpow(10, 16)-1))
        return 16;
    else 
        return -1;
    
    return 10;
    
 }

//number to exponent, returns long long value 
long long longpow(int a, int b)
{
    return (long long) pow(a, b);
}

//returns the digit on the index in the credit card number
int number_on_index(long long cc_number, int index)
{
    int n = digits_number(cc_number);
    if (n == -1 || index > n)
        return -1;
    else
    {
        long long remainder = cc_number % longpow(10, (n - index));
        remainder = (cc_number - remainder) / longpow(10, (n - index));
        return remainder % 10;
        
    }
}

//
int checksum(long long cc_number)
{
    int dn = digits_number(cc_number);
    if (dn == -1)
        return -1;
    int n = dn % 2;
    int index1, index2;
    
    int digit1, digit2;
    int sum1 = 0, sum2 = 0;
    
    if ( n == 1)
    {
        index1 = 2, index2 = 1;
        
    }
    else 
    {
        index1 = 1, index2 = 2;
    }
    
    while (index1 < dn)
        {
            digit1 = number_on_index(cc_number, index1) * 2;
            if (digit1 > 9)
            {
                //the max is 9*2 = 18, the sum of digits if 18%10=8, 8+1 = 9
                digit1 = (digit1 % 10) + 1;
            }
            sum1 += digit1;
            index1 += 2;
        }
        
        while (index2 <= dn)
        {    
            digit2 = number_on_index(cc_number, index2);
            sum2 += digit2;
            index2 += 2;
        }
    return sum1 + sum2;
}

void validation(long long cc_number)
{
    int cs = checksum(cc_number) % 10;
    int dn = digits_number(cc_number);
    int n1 = number_on_index(cc_number, 1), n2 = number_on_index(cc_number, 2);
    if ( cs == 0 && dn != -1)
    {
    if (dn == 15 && (n1 == 3) && (( n2 == 4) || (n2 == 7) ))
        printf("AMEX\n");
    else if(dn == 16 && n1 == 5 && (n2 >= 1 && n2 <= 5))
        printf("MASTERCARD\n");
    else if((dn == 13 || dn == 16) && n1 == 4)
        printf("VISA\n");
    }
    else
        printf("INVALID\n");
}