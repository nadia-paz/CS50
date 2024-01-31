#include <stdio.h>
#include <cs50.h>

void printspaces(int i);
void printhashes(int i);

int main(void)
{
    
    int height;
    do
    {
      printf("Height: ");
      height = GetInt();
    }
    while (height < 0 || height > 23);
    
    for (int i = 1; i <= height; i++)
    {
        int spaces = height - i;
        
        printspaces(spaces);
        printhashes(i);
        printspaces(2);
        printhashes(i);
        printf("\n");
        
    }
    
}

void printspaces(int i)
{
    int n = 0;
    while (n < i)
    {
      printf(" ");  
      n++;
    }
}

void printhashes(int i)
{
    int n = 0;
    while (n < i)
    {
      printf("#");
      n++;
    }
}