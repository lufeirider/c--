#include <windows.h>
#include <stdio.h>

int main()
{
	FILE *fp;
	int age;
	char name[10];
	int sex;

	if((fp = fopen("c:\\test.txt","a+"))==NULL)
	{
		printf("open error");
	}

	fscanf(fp,"%d %s %d",&age,name,&sex);
	printf("%d %s %d",age,name,sex);

	char info[]="22 test 1";
	fwrite(info,1,strlen(info),fp);

	fclose(fp);

	return 0;
}
