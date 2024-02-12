/**
 * Resizes a BMP by the argument.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize infile outfile\n");
        return 1;
    }

    // remember filenames
    float resize;
    sscanf(argv[1], "%3f", &resize);
    //printf("Resize is %f\n", resize);
    if(resize <= 0.0 || resize >= 100.0)
    {
        fprintf(stderr, "Resize must be between 0.0 and 100.0\n");
        return 1;
    }
    
    char* infile = argv[2];
    char* outfile = argv[3];
    
    if ( strstr(infile, ".bmp") == NULL || strstr(outfile, ".bmp") == NULL)
    {
        fprintf(stderr, "Please bmp files only\n");
        return 1;
    }

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // store original dimensions
    int originalWidth = abs(bi.biWidth);
    int originalHeight = abs(bi.biHeight);

    // change dimensions in BITMAPINFOHEADER
    bi.biWidth = roundf(bi.biWidth * resize);
    bi.biHeight = roundf(bi.biHeight * resize);
    
    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int originalPadding =  (4 - (originalWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //updating biSizeImage
    bi.biSizeImage = (abs(bi.biWidth) * 3 + padding) * abs(bi.biHeight);
    
    // change dimensions in BITMAPFILEHEADER
    bf.bfSize = bf.bfOffBits + bi.biSizeImage;
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    float hight_resize = resize, width_resize;
    int hight_track = 0, width_track;
    // iterate over infile's scanlines
    for (int i = 0; i < originalHeight; i++)
    {
         //no difference where to put fseek... before while or after. WHY?
         //fseek(inptr, bf.bfOffBits + i * (originalPadding + (originalWidth * 3)), SEEK_SET);
         while ( ((int) hight_resize) > hight_track )
         {
            fseek(inptr, bf.bfOffBits + i * (originalPadding + (originalWidth * 3)), SEEK_SET);
            // iterate over pixels in scanline
            width_track = 0;
            width_resize = resize;
            for (int j = 0; j < originalWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                while( ((int) width_resize) > width_track)
                {
                   fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                   width_track++;
                }
                width_resize += resize;
            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
                fputc(0x00, outptr);
            hight_track++;
           
        }//high_track
        hight_resize += resize;
    }//for height iteration 
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    
    // success
    return 0;
}
