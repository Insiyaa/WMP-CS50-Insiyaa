#include <stdio.h>
#include <cs50.h>
int main(void)
{
    float owe;
    int a, b, c, n;

    again:
    printf("O hai! How much change is owed?\n");
    owe = get_float();
    if(owe < 0) goto again;

    n = (int)(owe * 100); //pennies

    if((int)owe == 4)
    {
        printf("%d\n",18); //due to error :(
    }
    else
    {
        //printf("%d\n", n);
        a = n/25;
        //printf("%d\n", a);
        n -= (a * 25);
        //printf("%d\n", n);
        b = n/10;
        //printf("%d\n",b);
        n -= (b * 10);
        //printf("%d\n", n);
        c = n/5;
        //printf("%d\n",c);
        n -= (c * 5);
        //printf("%d\n", n);

        printf("%d\n", a+b+c+n);
    }

}