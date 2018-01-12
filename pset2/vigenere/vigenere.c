#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>

int keyed(char);
int main(int argc, char * argv[])
{
    //check if only two c-line arguments are entered
    if(argc != 2)
    {
        printf("Enter a command line argument\n");
        return 1;
    }
    string key = argv[1];
    //checking if there's any non-alphabetical character in key...
    int i = 0,k,j=0;
    while(key[i] != '\0')
    {
        if(!((key[i] >= 'a' && key[i] <= 'z')||(key[i] >= 'A' && key[i] <= 'Z')))
        {
            printf("Key should contain only alphabets...");
            return 1;
        }
        i++;
    }
    printf("plaintext: ");
    string text = get_string(); //input from user
    printf("ciphertext: ");
    int n = strlen(key);
    i = 0;
    int x = 0;
    while(text[i] != '\0') //iterating
    {
        //deciding value of k
        if(x < n)
        {
            k = keyed(key[x]);
        }
        else
        {
            j = x % n;
            k = keyed(key[j]);
        }
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
            x++;
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
            x++;
        }
        else
            printf("%c", text[i]);
        i++;
    }
    printf("\n");

}
int keyed(char key)
{
    if(key >= 'A' && key <= 'Z')
    {
        return key%65;
    }
    else
        return key%97;

}