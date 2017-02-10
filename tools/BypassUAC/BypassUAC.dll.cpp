// testdll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "windows.h"
#include <Tlhelp32.h>  
#include <algorithm>  
#include <vector> 

void kill(int pid)  
{  
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);  
    BOOL bOkey = TerminateProcess(hProcess, 0);  
    if(bOkey)  
    {  
        WaitForSingleObject(hProcess, 1000);  
        CloseHandle(hProcess);  
        hProcess = NULL;  
    }  
}  

void TerminateProcessByName(PCWSTR name)  
{  
    DWORD pid = 0;  
  
    // Create toolhelp snapshot.  
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
    PROCESSENTRY32W process;  
    ZeroMemory(&process, sizeof(process));  
    process.dwSize = sizeof(process);  
  
    std::vector<DWORD> pids;  
    // Walkthrough all processes.  
    if (Process32FirstW(snapshot, &process))  
    {  
        do  
        {  
            // Compare process.szExeFile based on format of name, i.e., trim file path  
            // trim .exe if necessary, etc.  
            std::wstring szExeFile = process.szExeFile;  
            std::wstring szName = name;  
            /*std::transform(szExeFile.begin(), szExeFile.end(), szExeFile.begin(), towlower); 
            std::transform(szName.begin(), szName.end(), szName.begin(), towlower);*/  
  
            if (!wcscmp(szExeFile.c_str(), szName.c_str()))  
            {  
                pid = process.th32ProcessID;  
                pids.push_back(pid);  
            }  
        } while (Process32NextW(snapshot, &process));  
    }  
  
    CloseHandle(snapshot);  
  
    size_t size = pids.size();  
    for(size_t i = 0; i < size; ++i)  
    {  
        kill(pids[i]);  
    }  
}  

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	MessageBox(NULL,TEXT("lufeitest"),TEXT("lufeitest"),MB_ICONINFORMATION|MB_YESNO);
	TerminateProcessByName(L"InfDefaultInstall.exe");
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	 }
    return TRUE;
}

