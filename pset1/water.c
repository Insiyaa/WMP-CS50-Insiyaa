#include<stdio.h>
#include<cs50.h>

int main(void)
{
    int min;
    printf("Minutes: ");
    min = get_int();
    printf("Bottles: %d\n",min*12);
}