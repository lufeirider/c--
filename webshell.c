#include "stdafx.h"
#include <windows.h>
#include <stdio.h>


int _tmain(int argc, _TCHAR* argv[])
{
	SECURITY_ATTRIBUTES		sa;
	HANDLE					hRead, hWrite;
	byte					buf[40960] = { 0 };
	STARTUPINFOW			si;
	PROCESS_INFORMATION		pi;
	DWORD					bytesRead;
	RtlSecureZeroMemory(&si, sizeof(si));
	RtlSecureZeroMemory(&pi, sizeof(pi));
	RtlSecureZeroMemory(&sa, sizeof(sa));
	int br = 0;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		ExitProcess(0);
	}
	wprintf(L"Trying to execute %s as SYSTEM\n", argv[1]);
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfoW(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.lpDesktop = L"WinSta0\\Default";
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	wchar_t cmd[4096] = { 0 };
	lstrcpyW(cmd, argv[1]);
	if (!CreateProcessW(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(hWrite);
		CloseHandle(hRead);
		wprintf(L"CreateProcessW Failed![%p]\n",GetLastError());
		ExitProcess(0);
	}
	CloseHandle(hWrite);
	printf("ProcessCreated with pid %d!\n", pi.dwProcessId);
	while (1)
	{
		if (!ReadFile(hRead, buf + br, 4000, &bytesRead, NULL))
			break;
		br += bytesRead;
	}

	puts((char*)buf);
	fflush(stdout);
	fflush(stderr);
	CloseHandle(hRead);
	CloseHandle(pi.hProcess);

	return 0;
}
