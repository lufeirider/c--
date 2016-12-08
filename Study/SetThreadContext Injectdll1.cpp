// NoProcessExeA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

typedef HMODULE (__stdcall *pLoadLibrary)(LPCSTR lpLibFileName);

void test()
{
    char dllname[] = {'c', ':', '\\', 't', 'e', 's', 't', '.', 'd', 'l', 'l', '\0'};
    //必须用2088770939这个 LoadLibrary 函数的绝对地址（在我的xp下是这个地址，使用时应该在自己的windows下获取）
    pLoadLibrary pFunc = pLoadLibrary(2088770939);
    //调用LoadLibrary，因为LoadLibrary函数在windows下每个进程中绝对地址都是一样的。
    pFunc(dllname);
}

int main(int argc, char *argv[])
{
    CONTEXT context;
    memset(&context, 0, sizeof(context));
    context.ContextFlags = CONTEXT_CONTROL;
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    BOOL bOk = CreateProcess(NULL,
                             "C:\\WINDOWS\\system32\\notepad.exe", // command line
                             NULL,          // process security attributes
                             NULL,          // primary thread security attributes
                             FALSE,          // handles are inherited
                             0,             // creation flags
                             NULL,          // use parent's environment
                             "C:\\WINDOWS\\system32\\",// use parent's current directory
                             &siStartInfo,  // STARTUPINFO pointer
                             &piProcInfo);  // receives PROCESS_INFORMATION

    ::WaitForInputIdle(piProcInfo.hProcess, -1);
    //为目标进程分配空间
    LPVOID pRemote = ::VirtualAllocEx(piProcInfo.hProcess, 0, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    DWORD dWriten = 0, id = 0;
    //将test函数写入目标进程
    ::WriteProcessMemory(piProcInfo.hProcess, pRemote, (LPVOID)test, 1024, &dWriten);
    //挂起目标进程的主线程
    ::SuspendThread(piProcInfo.hThread);
    //获取目标线程的CPU上下文信息
    ::GetThreadContext(piProcInfo.hThread, &context);
    //更改Eip指令为我们拷贝好的test函数
    context.Eip = (long)pRemote;
    //设置目标线程的CPU上下文信息
    ::SetThreadContext(piProcInfo.hThread, &context);
    //唤醒目标线程，
    ::ResumeThread(piProcInfo.hThread);
    //目标线程此时就会执行我们的test函数了，执行完后还会继续沿着
    //自己原先的代码序列执行下去。
    return 0;
}
