#include <stdio.h>
#include <cs50.h>
#include <ctype.h>

int main(void){
    string input;
    char c;
    input = get_string();
    int i = 0;
    printf("%c",toupper(input[0]));
    while(input[i] != '\0')
    {
        if(input[i] == ' ')
        {
            c = input[i+1];
            printf("%c",toupper(c));
        }

        i++;

    }
    printf("\n");
}