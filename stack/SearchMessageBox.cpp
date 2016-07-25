#include <Windows.h>
#include <stdio.h>
typedef void (*MYPROC)(LPTSTR);
int main()
{
	HINSTANCE LibHandle;
	MYPROC ProcAdd;
	LibHandle = LoadLibrary("user32");
	//获取user32.dll地址
	printf("user32 = 0x%x\n", LibHandle);
	//获取MessageBoxA的地址
	ProcAdd = (MYPROC)GetProcAddress(LibHandle,"MessageBoxA");
	printf("MessageBoxA = 0x%x", ProcAdd);
	getchar();
	return 0;
}