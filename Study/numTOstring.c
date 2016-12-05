#include<stdio.h>  
void main (void)
{
	char str[25];
	int num=100;
	sprintf(str, "%d" , num);
	printf ("%s",str);
}
