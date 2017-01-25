#include <stdio.h>
#include <windows.h>
void main()
{
	HANDLE isWait = CreateEvent(NULL,false,false,"test");
	WaitForSingleObject(isWait, INFINITE);
	printf("跨进程成功");
	getchar();
}
