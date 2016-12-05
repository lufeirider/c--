#include<windows.h>
#include<stdio.h>
int main()
{
	char path[100];
	GetSystemDirectory(path,100);
	printf("%s",path);
	return 0;
}
