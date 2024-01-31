 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
const int d = 4;
//int tr = 0;
//int tc = 0;
int value = 1;
int max_value = 15;
  
void draw(void);
//bool move(int tile);
bool won(void);

int main(void)
{
    //bool even = false;
    /*if (d % 2 == 0)
        even = true;*/
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            
            board[i][j] = value;
            
            /*if(even && value == 2)
                board[i][j] = 1;
            if(even && value == 1)
                board[i][j] = 2;*/
            
            if(value == 16)
                {
                    value = 30;
                    printf(" _");
                   // tr = i;
                   // tc = j;
                    
                }
            else
                printf("%2i ", board[i][j]);
            value++;
        }
        printf("\n");
        printf("\n");
    }
   // if(move(4))
     //   draw();
     
     if(won())
     {
         printf("You won!\n");
     }
    
}

void draw(void)
{
      printf("**************\n");
      for(int i = 0; i < d; i++)
      {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == 0)
                {
                    printf(" _");
                    //tr = i;
                   // tc = j;
                }
            else
                printf("%2i ", board[i][j]);
        }
        printf("\n");
        printf("\n");
      }
}

/*bool move(int tile)
{
    // TODO
   // bool legal = false;
    int mr = -1, mc = -2;
    for(int i = 0; i < d; i++)
      {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == tile)
            {
                printf("Tile %i, i is %i, j is %i\n", board[i][j], i, j);
                mr = i;
                mc = j;
                printf("mr = %i, mc = %i\n", mr, mc);
            }
            if(board[i][j] == 0)
            {
                printf("Zero %i position: i is %i, j is %i\n", board[i][j], i, j);
                tr = i;
                tc = j;
                printf("tr = %i, tc = %i\n", tr, tc);
            }
        }
      }
      printf("mr = %i, mc = %i\n", mr, mc);//2,3
      printf("tr = %i, tc = %i\n", tr, tc);//3,3
 
    if(mr >= 0 && mr <= d && mc >= 0 && mc <= d)
    {
        if( (mr == tr && (mc == tc - 1 || mc == tc + 1)) || 
            (mc == tc && (mr == tr - 1 || mr == tr + 1)))
        {
            board[mr][mc] = 0;
            board[tr][tc] = tile;
            return true;
        }
    }
    return false;
}*/

bool won(void)
{
    // TODO
    int count = 1;
    bool check = false;
    for(int i = 0; i < d; i++)
      {
        for(int j = 0; j < d; j++)
        {
            
            while(count < max_value)
            {
                if(count == board[i][j])
                {
                    count++;
                    check = true;
                }
                else
                {
                    check = false;
                    break;
                } 
            }
            
        }
      }
    return check;
}