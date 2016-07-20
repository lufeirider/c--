#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main()
{
	char strs[256]="";
	srand(time(0));
	int count = rand() % (3 + 1 - 0) + 0;
	for(int i=0;i<=count;i++)
	{
		srand(time(0)*1000000000);
		int num1 = rand() % (100 + 1 - 0) + 0;
		char str1[256];
		itoa(num1, str1, 10);
		strcat(strs, str1);
	}

	for(i=0;i<=count;i++)
	{
		srand(time(0)*10000000);
		int num2 = rand() % (100 + 1 - 0) + 0;
		char str2[25];
		itoa(num2, str2, 10);
		strcat(strs, str2);
	}
	strcat(strs, ".dat");
	printf(strs);

	char szcmd[256] = "cmd /c rename ";
	strcat(szcmd, "common.dat" );
	strcat(szcmd, " ");
	
	strcat(szcmd, strs);
	system(szcmd);
	return 0;
}
