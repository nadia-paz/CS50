/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if (n <= 0)
        return false;
    else
    {
        int middle, begin, end;
        begin = 0;
        end = n - 1;
        while ( begin <= end)
            {
            middle = (end + begin) / 2;
            if ( values[middle] == value )
                return true;
            else if ( values[middle] < value )
                {
                    begin = middle + 1;
                }
            else if (values[middle] > value )
                {
                    end = middle - 1;
                }
            }
       return false; 
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
   // implement an O(n) sorting algorithm
   //countin sort
   int maxval = 65536; // the same value as MAX
   int index[maxval+1];
   //fill the array with zeros
    for(int i = 0; i < maxval; i++)
    {
        index[i] = 0;
    }
    //increment values of index array
    //and clean the values array
    for(int j = 0; j < n; j++)
    {
        ++index[values[j]];
        values[j] = 0;
    }
    
    //if the index is > 0; out it into the array of values
    for(int i = 0, iv = 0; i < maxval; i++)
    {
       while(index[i] > 0)
        {
            values[iv] = i;
            ++iv;
            --index[i];
        }
    }
    
}
