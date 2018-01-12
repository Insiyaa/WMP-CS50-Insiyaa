#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int h = -1,i,j;
    while(!(h<24 && h>=0))
    {
        printf("Height:");
        h = get_int();
    }
    /*
       ##
      ###
     ####
    #####
    */
    for(i = 1; i <= h;i++)
    {
        //space
        for(j = i; j < h; j++)
            printf(" ");
        printf("#");
        for(j = 1; j <= i; j++)
            printf("#");
        printf("\n");
    }
}