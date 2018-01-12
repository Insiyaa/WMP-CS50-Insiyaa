#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    int k,i=0,n;
    //check if only two c-line arguments are entered
    if(argc != 2)
    {
        printf("Enter a command line argument\n");
        return 1;
    }
    //argv is a char array, change it to integer
    n = atoi(argv[1]);
    if(n >= 26)
    {
        k = n % 26; //appropriate value of k
    }
    else
    {
        k = n;
    }
    printf("plaintext: ");
    string text = get_string(); //input from user
    printf("ciphertext: ");
    while(text[i] != '\0') //iterating
    {
        //if character is lowercase alphabet...
        if((text[i] >= 'a' && text[i] <= 'z'))
        {
            //If exceeds alphabets... subtract 26
            if((text[i] + k) > 122)
            {
                printf("%c", (text[i] + k) - 26) ;
            }
            else
                printf("%c", text[i] + k);
        }
        //if character is an upperCase alphabet...
        else if(text[i] >= 'A' && text[i] <= 'Z')
        {
            if((text[i] + k) > 90)
            {
                printf("%c", (text[i] + k) - 26) ;
            }
            else
                printf("%c", text[i] + k);
        }
        else
            printf("%c", text[i]);
        i++;
    }
    printf("\n");

}