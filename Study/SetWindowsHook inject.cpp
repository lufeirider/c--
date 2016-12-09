#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <tlhelp32.h>

bool InjectDll(const TCHAR* pszDllFile, DWORD dwProcessId)
{
     if (NULL == pszDllFile || 0 == ::_tcslen(pszDllFile))
     {
      return false;
     }
 
     HANDLE hProcess  = NULL;
     HANDLE hThread   = NULL;
     DWORD dwSize   = 0;
     TCHAR* pszRemoteBuf = NULL;
     LPTHREAD_START_ROUTINE lpThreadFun = NULL;
 
     // 获取目标进程句柄
     hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessId);
     if (NULL == hProcess)
     {
      return false;
     }
 
     // 在目标进程中分配内存空间
     dwSize = (DWORD)::_tcslen(pszDllFile) + 1;
     pszRemoteBuf = (TCHAR*)::VirtualAllocEx(hProcess, NULL, dwSize * sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);
     if (NULL == pszRemoteBuf)
     {
      ::CloseHandle(hProcess);
      return false;
     }
 
     // 在目标进程的内存空间中写入所需参数(模块名)
     if (FALSE == ::WriteProcessMemory(hProcess, pszRemoteBuf, (LPVOID)pszDllFile, dwSize * sizeof(TCHAR), NULL))
     {
      ::VirtualFreeEx(hProcess, pszRemoteBuf, dwSize, MEM_DECOMMIT);
      ::CloseHandle(hProcess);
      return false;
     }
 
     // 从 Kernel32.dll 中获取 LoadLibrary 函数地址
    #ifdef _UNICODE
     lpThreadFun = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "LoadLibraryW");
    #else
     lpThreadFun = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "LoadLibraryA");
    #endif
     if (NULL == lpThreadFun)
     {
      ::VirtualFreeEx(hProcess, pszRemoteBuf, dwSize, MEM_DECOMMIT);
      ::CloseHandle(hProcess);
      return false;
     }
 
     // 创建远程线程调用 LoadLibrary
     hThread = ::CreateRemoteThread(hProcess, NULL, 0, lpThreadFun, pszRemoteBuf, 0, NULL);
     if (NULL == hThread)
     {
      ::VirtualFreeEx(hProcess, pszRemoteBuf, dwSize, MEM_DECOMMIT);
      ::CloseHandle(hProcess);
      return false;
     }
 
     // 等待远程线程结束
     ::WaitForSingleObject(hThread, INFINITE);
 
     // 清理
     ::VirtualFreeEx(hProcess, pszRemoteBuf, dwSize, MEM_DECOMMIT);
     ::CloseHandle(hThread);
     ::CloseHandle(hProcess);
 
 return true;
}
 
 
bool UnInjectDll(const TCHAR* pszDllFile, DWORD dwProcessId)
{
 // 参数无效
     if (NULL == pszDllFile || 0 == ::_tcslen(pszDllFile))
     {
      return false;
     }
 
     HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
     HANDLE hProcess = NULL;
     HANDLE hThread  = NULL;
 
     // 获取模块快照
     hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
     if (INVALID_HANDLE_VALUE == hModuleSnap)
     {
      return false;
     }
 
     MODULEENTRY32 me32;
     memset(&me32, 0, sizeof(MODULEENTRY32));
     me32.dwSize = sizeof(MODULEENTRY32);
 
     // 开始遍历
     if(FALSE == ::Module32First(hModuleSnap, &me32))
     {
      ::CloseHandle(hModuleSnap);
      return false;
     }
 
     // 遍历查找指定模块
     bool isFound = false;
     do
     {
      isFound = (0 == ::_tcsicmp(me32.szModule, pszDllFile) || 0 == ::_tcsicmp(me32.szExePath, pszDllFile));
      if (isFound) // 找到指定模块
      {
       break;
      }
     } while (TRUE == ::Module32Next(hModuleSnap, &me32));
 
     ::CloseHandle(hModuleSnap);
 
     if (false == isFound)
     {
      return false;
     }
 
     // 获取目标进程句柄
     hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION, FALSE, dwProcessId);
     if (NULL == hProcess)
     {
      return false;
     }
 
     // 从 Kernel32.dll 中获取 FreeLibrary 函数地址
     LPTHREAD_START_ROUTINE lpThreadFun = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "FreeLibrary");
     if (NULL == lpThreadFun)
     {
      ::CloseHandle(hProcess);
      return false;
     }
 
     // 创建远程线程调用 FreeLibrary
     hThread = ::CreateRemoteThread(hProcess, NULL, 0, lpThreadFun, me32.modBaseAddr /* 模块地址 */, 0, NULL);
     if (NULL == hThread)
     {
      ::CloseHandle(hProcess);
      return false;
     }
 
     // 等待远程线程结束
     ::WaitForSingleObject(hThread, INFINITE);
 
     // 清理
     ::CloseHandle(hThread);
     ::CloseHandle(hProcess);
 
     return true;
}

int main(int argc, char* argv)
{
	InjectDll("c:\\test.dll",788);
	return 0;
}
