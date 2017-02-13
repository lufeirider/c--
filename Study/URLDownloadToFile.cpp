#include <stdio.h>
#include <windows.h>

int main()
{
	char * szPath = "c:\\test.exe";
	char * szUrl = "file://c:\\windows\\system32\\cmd.exe";
	HINSTANCE hi;
	hi = LoadLibrary("urlmon.dll");
	PVOID Down = GetProcAddress(GetModuleHandle("urlmon.dll"),"URLDownloadToFileA");
	__asm
	{
		push 0
		push 0
		push szPath
		Push szUrl
		push 0
		call Down
	}
	FreeLibrary(hi);

	hi = LoadLibrary("kernel32.dll");
	PVOID Run= GetProcAddress(GetModuleHandle("kernel32.dll"),"WinExec");
	__asm
	{
		push SW_SHOW
		push szPath
		call Run
	}
	FreeLibrary(hi);
    return 0;
}
