#include <stdio.h>
#include <stdlib.h>

main()
{
	FILE *fp;
	char ch;
	if((fp=fopen("test.txt","w"))==NULL)
	{
		printf("error\n");
		exit(0);
	}

	while ((ch=getchar())!='\n')
	fputc(ch, fp);

	fclose(fp);
}
/*
小提示：

fp=fopen("test.txt","w") ，把"w"改为 "a" 可以创建文件并且追加写入内容

exit(0);  需要包含 stdlib.h 头文件，才能使用
*/
