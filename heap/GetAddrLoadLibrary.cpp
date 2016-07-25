#include <windows.h>
#include <stdio.h>
typedef void (*MYPROC)(LPTSTR);
int main()
{	
		HINSTANCE LibHandle;
		MYPROC ProcAdd;
  		LibHandle = LoadLibrary("kernel32");
		printf("msvcrt LibHandle = //x%x\n", LibHandle);
		ProcAdd=(MYPROC)GetProcAddress(LibHandle,"LoadLibraryA");	
		printf("LoadLibrary = //x%x\n", ProcAdd);
		return 0;
}
