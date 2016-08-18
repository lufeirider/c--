#include <stdio.h>
#include <stdlib.h>
main( int argc, char *argv[] )
{
	char ch;
	FILE *fp;
	int i;

	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("不能打开文件\n");
		exit(0);
	}

	while ((ch=fgetc(fp))!=EOF)
	putchar(ch);

	fclose(fp);
}
