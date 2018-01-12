#include <stdio.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    // Check proper usage
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover file_name\n");
        return 1;
    }

    // Opening input file
    FILE *inptr = fopen(argv[1], "r");

    // VAlidate input file
    if(inptr == NULL)
    {
        fprintf(stderr, "Can't open file %s\n", argv[1]);
        return 2;
    }


    int i = 0; //to keep track of jpeg files
    unsigned char buffer[512];
    int found = 0;
    FILE *img = NULL;

    // Continue till EOF is encountered. That is when a single element of 512 bytes can't be read.
    while(fread(buffer, 512, 1, inptr) == 1)
    {
        // Detecting jpeg file
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If previously a file was open then close it and create new.
            if(found == 1)
            {
                fclose(img);
            }
            else // Not open then create new.
            {
                found = 1;
            }

            // Create new file
            char filename[15];
            sprintf(filename, "%03d.jpg", i);
            img = fopen(filename, "w");
            i++;
        }
        //If a file is open, and buffer is not start of jpeg, continue writing
        if(found == 1)
        {
            fwrite(buffer, 512, 1, img);
        }
    }

    fclose(inptr);
    fclose(img);

    return 0;

}
