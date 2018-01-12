#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int key(char);
int main(int argc, char argv[])
{
	//variables...
	int step, len, i = 0, track = 0, pos = 0, inc;
	char str[500], ciph[20];
	
	//checking valid execution of program...
	if(argc != 2 || argc != 3)
	{
		printf("./stepCi key [step]\n");
		return 1;
	}
	
	//if single argument provided, default step is 0...
	if (argc == 2)
	{
		step = 0;
	}
	else
	{
		step = atoi(argv[2]);
	}
	len = strlen(argv[1]);
	//converting step to valid number...
	
	if(step >= len)
	{
		step = step % len;
	}
	strcpy(ciph, argv[1]);
	//starting the program...
	printf("Plain text: ");
	gets(str);
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
			track++;
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
			track++;
		}
		else
		{
			printf("%c",str[i]);
		}
		i++;
	}
	printf("\n");
	
	
	
} 
int key(char c)
{
	if(c >= 'a' && c <= 'z')
		return c % 97;
	else
		return c % 65;
}
