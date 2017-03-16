#include <stdio.h>
#include <windows.h>

DWORD WINAPI ThreadProc(LPVOID pM)  
{  
    printf("1111111111");
    return 0;  
}  

void main()
{
	HANDLE handle1=CreateThread(NULL,0,ThreadProc,NULL,0,NULL); 
	ExitThread(0);
	getchar();
}
