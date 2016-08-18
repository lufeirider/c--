#include <stdio.h>
#include <stdlib.h>
#include <string.h>
main()
{
	FILE *fp;
	int i;
	struct student{
		char name[10];
		int age;
	}stu;

	if((fp=fopen("test.txt","wb"))==NULL)
	{
		printf("不能打开文件");
		exit(0);
	}
	
	//stu.name = "test"; 这样是错误的不能将char[10]转成char*
	//scanf("%s",stu.name);这样可以
	//char name[10]="test";这样可以
	strcpy(stu.name,"111");
	stu.age = 10;

	fwrite(&stu.name,strlen(stu.name),1,fp);
	//buff size count(一般为1) fp

	char buffer[] = { 'x' , 'y' , 'z' };
	fwrite (buffer , sizeof(char), sizeof(buffer), fp);
	

	fclose(fp);
}
