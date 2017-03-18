#include <stdio.h>
#include <windows.h>


void main()
{
	printf("111111111111111111");
	
	__asm
	{
		push edx
		pop edx
		inc eax
		dec eax
		rdtsc
		push edx
		pop edx
		inc eax
		dec eax
		rdtsc
		push edx
		pop edx
		inc eax
		dec eax
	}

	printf("222222222222222222");
	getchar();
}
