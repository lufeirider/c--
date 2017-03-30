#include <stdio.h>
#include <windows.h>

int main()
{
	char* test = "AAAAAAA";
	char* p = NULL;

	p =(char*) malloc(strlen(test));
	p[strlen(test)] = 0x00;

	for(unsigned int i=0;i<strlen(test);i++)
	{
		p[i] = test[i]^0x01;
	}

	printf("%s\n",test);
	printf("%s\n",p);
	return 0;
}
