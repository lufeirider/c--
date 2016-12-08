// NoProcessExeA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>

/************************************************************************/
/* 功能:取得指定进程ID
/* 参数:进程名
/* 返回值:指定进程ID
/************************************************************************/
DWORD GetProcessId(char* Name)
{
	int i = 1000;
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32 );
	HANDLE Handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	Process32First(Handle,&pe);
	int x = strcmp(Name,pe.szExeFile);
	if (x == 0)
	{
		return pe.th32ProcessID;
	}
	while (i > 0)
	{
		Process32Next(Handle,&pe);
		x = strcmp(Name,pe.szExeFile);
		if (x == 0)
		{
			return pe.th32ProcessID;
		}
		i--;
	}
	return 0;
}

/************************************************************************/
/* 功能:取得指定进程的主线程ID
/* 参数:指定进程的进程ID
/* 返回值:指定进程的主线程ID
/************************************************************************/
DWORD GetThreadId(DWORD dwProcessId)
{
	DWORD dwThreadID = 0;
	THREADENTRY32 te32 = {sizeof(te32)};
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,dwProcessId);
	if( Thread32First( hThreadSnap, &te32) )
	{
		do{
			if( dwProcessId == te32.th32OwnerProcessID )
			{
				dwThreadID = te32.th32ThreadID;
				break;
			}
		}while( Thread32Next( hThreadSnap, &te32) );
	}
	
	return dwThreadID;
}

int main(int argc, char* argv[])
{
	DWORD		dwProcessId;
	HANDLE		hProcess;
	LPVOID		ParameterAddress;
	char		DllName[100] = {0};
	strcat(DllName,"C:\\NoProcessDllA.dll");
	DWORD		dwWirite;
	LPVOID		lpLibraryAddress;
	HANDLE		hRemote;
//////////////////////////////////////////////////////////////////////////变量区

	dwProcessId = GetProcessId("notepad.exe");
	if (dwProcessId == NULL)
	{
		printf("GetProcessId Error \n");
		return 0;
	}

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessId);
	if (hProcess == NULL)
	{
		printf("OpenProcess Error \n");
		return 0;
	}

	ParameterAddress = VirtualAllocEx(hProcess,NULL,200,MEM_COMMIT,PAGE_READWRITE);
	if (ParameterAddress == NULL)
	{
		printf("VirtualAllocEx Error \n");
		return 0;
	}

	if (! WriteProcessMemory(hProcess,ParameterAddress,(LPCVOID)DllName,strlen(DllName),&dwWirite))
	{
		printf("WriteProcessMemory Error \n");
		return 0;
	}

	lpLibraryAddress = GetProcAddress(LoadLibraryA("Kernel32.dll"),"LoadLibraryA");
	if (lpLibraryAddress == NULL)
	{
		printf("GetProcAddress + LoadLibraryA Error \n");
		return 0;
	}

	hRemote = CreateRemoteThread(hProcess,NULL,0,(LPTHREAD_START_ROUTINE)lpLibraryAddress,ParameterAddress,0,NULL);
	if (hRemote == NULL)
	{
		printf("CreateRemoteThread Error \n");
		return 0;
	}

	return 0;
}
