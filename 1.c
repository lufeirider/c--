#include<stdio.h>
#include<stdlib.h>
void main()
{
	unsigned char a;
	int i;
	FILE *fp,*fp1;
	fp=fopen("c:\\1.exe","rb+");
	fp1=fopen("c:\\2.exe","wb+");

	if(fp==NULL)
	{
		printf("open file error!\n");
		return;
	}

	if(fp1==NULL)
	{
		printf("open file1 error!\n");
		return;
	}

	while(!feof(fp))
	{
		ftell(fp);
		int length=ftell(fp);
		printf("%x",length);
		if(length==2)
		{
			fread(&a,1,1,fp);
			int b=0x99;
			fwrite(&b, 1, 1, fp1);
		}else{
			fread(&a,1,1,fp);
			int b=(int)a;
			printf("%02x",a);
			fwrite(&b, 1, 1, fp1);
		}
	}

	printf("\n");
	fclose(fp);
	fclose(fp1);
}

