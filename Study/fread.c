#include <string.h>
#include <stdio.h>
int main(void)
{
	FILE *stream;
	char msg[] = "this is a test";
	char buf[20];
	if ((stream = fopen("test.txt", "w+"))== NULL)
	{
		fprintf(stderr,"Cannot open output file.\n");
		return 1;
	}
	/* 写入数据到文件中 */
	fwrite(msg, strlen(msg)+1, 1, stream);
	//strlen(msg)+1写入的时候多加了0000
	/* 将文件文件指针知道文件开始 */
	fseek(stream, SEEK_SET, 0);
	/* 读取文件里面的内容 */
	fread(buf, strlen(msg)+1, 1, stream);
	//strlen(msg)+1读取的时候多加了0000
	printf("%s\n", buf);
	fclose(stream);
	return 0;
}
