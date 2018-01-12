#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
int key(char);
int main(void)
{
	//variables...
	int step, len, i = 0, pos = 0, inc;
	char str[500], ciph[20];
	
	//Input key and code...
	printf("Enter code: ");
	gets(ciph);
	printf("Enter key: ");
	scanf("%d", &step);
	printf("Plain text: ");
	scanf("%s",str);
	//gets(str);
	len = strlen(ciph);
	
	//converting step to valid number...
	if(step >= len)
	{
		step = step % len;
	}
	
	//starting the program...
	printf("Cipher text: ");
	while(str[i] != '\0')
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
		{
			inc = key(ciph[pos]);
			if((str[i] + inc) > 'Z')
				printf("%c", str[i] + inc - 26);
			else
				printf("%c", str[i] + inc);
			pos = pos + (1 + step);
			if(pos >= len)
				pos = pos % len;
		}
		else if(str[i] >= 'a' && str[i] <= 'z')
		{
			inc = key(ciph[pos]);
			if((str[i] + inc) > 'z')
				printf("%c", str[i] + inc - 26);
			else
				printf("%c", str[i] + inc);
			pos = pos + (1 + step);
			if(pos >= len)
				pos = pos % len;
		}
		else
		{
			printf("%c",str[i]);
		}
		i++;
	}
	printf("\n");
	return 0;
	
	getch();
	
} 
int key(char c)
{
	if(c >= 'a' && c <= 'z')
		return c % 97;
	else
		return c % 65;
}
