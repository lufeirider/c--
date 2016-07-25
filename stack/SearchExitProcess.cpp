#include <Windows.h>
#include <stdio.h>
typedef void (*MYPROC)(LPTSTR);
int main()
{
	HINSTANCE LibHandle;
	MYPROC ProcAdd;
	LibHandle = LoadLibrary("kernel32");
	//获取kernel32.dll的地址
	printf("kernel32 = 0x%x\n", LibHandle);
	//获取ExitProcess的地址
	ProcAdd = (MYPROC)GetProcAddress(LibHandle, "ExitProcess");
	printf("ExitProcess = 0x%x\n", ProcAdd);
	getchar();
	return 0;
}