#include <stdio.h> 
#include <windows.h> 
void main() 
{ 
    unsigned int sehaddr; 
    int *un; 
    HMODULE hk = LoadLibrary("KERNEL32.dll"); 
    un = (int *)GetProcAddress(hk,"SetUnhandledExceptionFilter"); 
    _asm
	{ 
        mov eax,un 
        add eax,5 
        mov ebx,[eax] 
        mov sehaddr,ebx 
    } 
    printf("the top seh: 0x%x\r\n",sehaddr); 
    _getch(); 
    return; 
} 

