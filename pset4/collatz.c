#include <stdio.h>

int collatz(int n)
{
    if (n < 1)
        return -500;
    if (n == 1)
        return 0;
   /* else if (n % 2 == 0)
        collatz(3 * (n - 1) + 1);
    else
        collatz((n - 1) / 2);*/
    else if( n % 2 == 0)
        return 1 + collatz(n / 2);
    else
        return 1 + collatz(n * 3 + 1);
    
}

int main(void)
{
    int n = collatz(6);
    printf("%i \n", n);
}