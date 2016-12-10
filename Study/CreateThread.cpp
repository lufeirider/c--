#include <stdio.h>   
#include <windows.h>   

DWORD WINAPI ThreadFun(LPVOID pM)  
{  
    printf("ID:%d\n", GetCurrentThreadId());  
    return 0;  
}  

int main()  
{
    HANDLE handle = CreateThread(NULL, 0, ThreadFun, NULL, 0, NULL);  
    WaitForSingleObject(handle, INFINITE);  
    return 0;  
}  
