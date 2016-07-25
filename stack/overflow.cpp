#include "stdio.h"
#include "string.h"

char name[] = "jiangyejiangye";

int main()
{
	char buffer[8];
	strcpy(buffer, name);
	printf("%s\n",buffer);
	getchar();
	return 0;
}