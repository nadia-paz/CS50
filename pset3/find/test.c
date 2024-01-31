#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int main(void)
{
    int values[] = {10, 4, 5, 3, 1, 0, 9, 8, 8, 2};
    int size = sizeof(values)/sizeof(int);
    int index[10+1];
    for(int i = 0; i < 11; i++)
    {
        index[i] = 0;
    }
    for(int j = 0; j < size; j++)
    {
        ++index[values[j]];
        values[j] = 0;
    }
    for(int i = 0; i < 11; i++)
    {
        printf("Index[%i] = %i\n", i, index[i]);
    }
    for(int i = 0, n = 0; i < 11; i++)
    {
       while(index[i] > 0)
        {
            values[n] = i;
            ++n;
            --index[i];
        }
    }
    for(int i = 0; i < size; i++)
    {
        printf("Values[%i] = %i\n", i, values[i]);
    }
}