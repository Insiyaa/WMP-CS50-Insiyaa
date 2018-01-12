#include <stdio.h>
#include <cs50.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit in_file out_file\n");
        return 1;
    }

    char *infile = argv[1];
    char *outfile = argv[2];

    FILE *inptr = fopen(infile, "r");
    if(inptr == NULL)
    {
        fprintf(stderr, "Could'nt open %s.\n",infile);
        return 2;
    }

    FILE *outptr = fopen(outfile, "w");
    if(outptr == NULL)
    {
        fprintf(stderr, "Could'nt read %s.\n",outfile);
        return 3;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    //read infile's BITMAPINFOHEADER...
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    //ensure infile is a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            //tweaking
            if (triple.rgbtRed == 255) {
                triple.rgbtRed = 255;
                triple.rgbtBlue = 255;
                triple.rgbtGreen = 255;
            }
            if (triple.rgbtRed < 240 && triple.rgbtRed > 225) {
                triple.rgbtRed = 0;
                triple.rgbtBlue = 0;
                triple.rgbtGreen = 0;
            } else if (triple.rgbtRed < 225 && triple.rgbtRed > 200) {
                triple.rgbtRed = 0;
                triple.rgbtBlue = 0;
                triple.rgbtGreen = 0;
            } else if (triple.rgbtRed < 200 && triple.rgbtRed > 175) {
                triple.rgbtRed = 125;
                triple.rgbtBlue = 125;
                triple.rgbtGreen = 125;
            } else if (triple.rgbtRed <= 175 && triple.rgbtRed > 150) {
                triple.rgbtRed = 60;
                triple.rgbtBlue = 60;
                triple.rgbtGreen = 60;
            } else if (triple.rgbtRed <= 150 && triple.rgbtRed > 100) {
                triple.rgbtRed = 50;
                triple.rgbtBlue = 50;
                triple.rgbtGreen = 50;
            } else if (triple.rgbtRed <= 150 && triple.rgbtRed > 100) {
                triple.rgbtRed = 25;
                triple.rgbtBlue = 25;
                triple.rgbtGreen = 25;
            } else if (triple.rgbtRed <= 100 && triple.rgbtRed > 50) {
                triple.rgbtRed = 15;
                triple.rgbtBlue = 15;
                triple.rgbtGreen = 15;
            } else if (triple.rgbtRed <= 50 && triple.rgbtRed > 0) {
                triple.rgbtRed = 0;
                triple.rgbtBlue = 0;
                triple.rgbtGreen = 0;
            }
            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    return 0;
}