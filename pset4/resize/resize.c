#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    //ensuring proper usage
    if(argc != 4)
    {
        fprintf(stderr,"Usage: ./resize n image_name.bmp resized_name.bmp\n");
        return 1;
    }

    //Checking value of n
    int n = atoi(argv[1]);

    if(n < 0 || n > 100)
    {
        fprintf(stderr, "n should be positive and <= 100\n");
    }

    //reading input and output filenames
    char *input = argv[2];
    char *output = argv[3];

    // Open input file
    FILE *inptr = fopen(input, "r");
    if(inptr == NULL)
    {
        fprintf(stderr,"Can't open %s.\n",input);
        return 2;
    }

    // Open output file
    FILE *outptr = fopen(output, "w");
    if(outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr,"Can't open %s.\n",output);
        return 3;
    }

    // Reading file headers
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Validation if it's BITMAP image
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Output file headers
    BITMAPFILEHEADER bfo;
    BITMAPINFOHEADER bio;
    bfo = bf;
    bio = bi;

    // scaling height and width
    bio.biWidth = n * bi.biWidth;
    bio.biHeight = n * bi.biHeight;

    // Calculating input and output padding
    int paddingi = (int)((4 - ((bi.biWidth) * sizeof(RGBTRIPLE)) % 4) % 4);
    int paddingo = (int)((4 - ((bio.biWidth) * sizeof(RGBTRIPLE)) % 4) % 4);

    // bfSize is size of bitmap file(multiplying with 3 to convert in bytes).
    bfo.bfSize = 54 + bio.biWidth * abs(bio.biHeight) * 3 + abs(bio.biHeight) *  paddingo;

    // I can't understand this formula but works fine, source: stack overflow
	bio.biSizeImage = ((((bio.biWidth * bio.biBitCount) + 31) & ~31) / 8) * abs(bio.biHeight);

    // Output file cursor at the beginning
    fseek(outptr, 0, SEEK_SET);
    fwrite(&bfo, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bio, sizeof(BITMAPINFOHEADER), 1, outptr);
    //printf("%d %d %d %d %d %d %d %d\n",bi.biWidth, bi.biHeight, bi.biSizeImage, bf.bfSize, bio.biWidth, bio.biHeight, bio.biSizeImage, bfo.bfSize);

    // Iterating through rows
    for(int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // Writing each row n times...
        for(int m = 0; m < n; m++)
        {
            // Iterating through columns...
            for(int j = 0, biWidth = abs(bi.biWidth); j < biWidth; j++)
            {

                RGBTRIPLE triple;
                    // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // Writing each pixel n times...
                for(int k = 0; k < n; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // Skipping input padding
            fseek(inptr, paddingi, SEEK_CUR);

            // Writing output padding
            for (int k = 0; k < paddingo; k++)
            {
                fputc(0x00, outptr);
            }

            // Going back to start of row (no. of bytes)... That is crossing over width plus padding
            // Negative Value suggests going backwards...
            fseek(inptr, -(bi.biWidth * 3 + paddingi ), SEEK_CUR);
        }

        // Back to end of row...
        fseek(inptr, bi.biWidth*3+paddingi, SEEK_CUR);
    }
    //close input file
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;


}