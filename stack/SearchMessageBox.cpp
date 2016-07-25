#include <Windows.h>
#include <stdio.h>
typedef void (*MYPROC)(LPTSTR);
int main()
{
	HINSTANCE LibHandle;
	MYPROC ProcAdd;
	LibHandle = LoadLibrary("user32");
	//��ȡuser32.dll��ַ
	printf("user32 = 0x%x\n", LibHandle);
	//��ȡMessageBoxA�ĵ�ַ
	ProcAdd = (MYPROC)GetProcAddress(LibHandle,"MessageBoxA");
	printf("MessageBoxA = 0x%x", ProcAdd);
	getchar();
	return 0;
}