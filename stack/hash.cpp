#include <stdio.h>
#include <windows.h>

DWORD GetHash(char *fun_name)
{
	DWORD digest = 0;
	while(*fun_name)
	{
		digest = ((digest << 25 ) | (digest >>7 ));
		digest += *fun_name;
		fun_name++;
	}
	return digest;
}

int main()
{
	DWORD hash;
	hash = GetHash("MessageBoxA");
	printf("The hash of MessageBoxA is 0x%.8x\n", hash);
	hash = GetHash("ExitProcess");
	printf("The hash of ExitProcess is 0x%.8x\n",hash);
	hash = GetHash("LoadLibraryA");
	printf("The hash of LoadLibraryA is 0x%.8x\n",hash);
	getchar();
}