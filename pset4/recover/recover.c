/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    typedef int boolean;
    #define true 1
    #define false 0
   
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    char* image = argv[1];
    FILE* img;
    
    boolean fileopen = false;
    int filename = 0;
    uint8_t buf[512];
    char* title = malloc(sizeof(uint8_t) * 8);
   
    FILE* inptr = fopen(image, "r");
    
    if (inptr == NULL) 
    {
        fprintf(stderr, "Could not open \"card.raw.\".\n");
        return 2;
    }
    
    while ( fread(&buf, 512, 1, inptr) > 0)
    {
        
       
        //when you find new jpg
        if ( buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff && (buf[3] == 0xe0 || buf[3] == 0xe1) )
        {
            //check if any of jpg files was created
            //in this case the filename should be more than 0
            sprintf(title, "%03d.jpg", filename);
            filename++;
            
            if ( fileopen == false )
            {
                //create the new jpg file
                img = fopen(title, "a");
                if (img == NULL)
                    {
                        fclose(inptr);
                        fprintf(stderr, "Could not create %s.\n", title);
                        return 3;
                    }
                fileopen = true;
            }
            else //if ( fileopen == true )
            {
                //close the previous file
                fclose(img);
                img = fopen(title, "a");
                if (img == NULL)
                    {
                        fclose(inptr);
                        fprintf(stderr, "Could not create %s.\n", title);
                        return 3;
                    }
             }
          
            //write into a file from 512 b bufer
            fwrite(&buf, sizeof(buf), 1, img);
        }//if jpgfinder
        else //jpgfinder
        {
           if (fileopen == true)
               fwrite(&buf, sizeof(buf), 1, img);
        }
    }//while !end
    
    if(img)
        fclose(img);
    
    fclose(inptr);
    free(title);
}