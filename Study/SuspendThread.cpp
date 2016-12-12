#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

typedef HANDLE (WINAPI *OPENTHREADFUN)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);


////-->定义OpenThread函数类型  别忘了WINAPI(API都是_stdcall类型)  
HMODULE hKernel32 = ::GetModuleHandle(_T("kernel32.dll"));

//2获得函数指针  
OPENTHREADFUN pFun = (OPENTHREADFUN)GetProcAddress(hKernel32, _T("OpenThread"));
//3调用  
//pFun(THREAD_ALL_ACCESS,FALSE,dwThreadId);  

BOOL ListProcessThreads( DWORD dwOwnerPID )
{
    HANDLE hProcess;
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 te32;

    // Take a snapshot of all running threads
    hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
    if( hThreadSnap == INVALID_HANDLE_VALUE )
        return( FALSE );

    // Fill in the size of the structure before using it.
    te32.dwSize = sizeof(THREADENTRY32);

    // Retrieve information about the first thread,
    // and exit if unsuccessful
    if( !Thread32First( hThreadSnap, &te32 ) )
    {
        CloseHandle( hThreadSnap );          // clean the snapshot object
        return( FALSE );
    }

    // Now walk the thread list of the system,
    // and display information about each thread
    // associated with the specified process
    do
    {
        if( te32.th32OwnerProcessID == dwOwnerPID )
        {

            if(GetCurrentThreadId() != te32.th32ThreadID)
            {
                _tprintf( TEXT("\n\n  SuspendThread   THREAD ID      = %d"), te32.th32ThreadID );
                hProcess = pFun( THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID  );
                SuspendThread(hProcess);
            }

            _tprintf( TEXT("\n"));
        }
    }
    while( Thread32Next(hThreadSnap, &te32 ) );

    CloseHandle( hThreadSnap );
    return( TRUE );
}

DWORD WINAPI ThreadFun(LPVOID pM)
{
    printf("ID:%d\n", GetCurrentThreadId());
    while(1)
    {
        printf("1111111");
    }
    return 0;
}

int main()
{

    HANDLE handle = CreateThread(NULL, 0, ThreadFun, NULL, 0, NULL);

    DWORD dwProcessId = GetCurrentProcessId();
    ListProcessThreads(dwProcessId);

    WaitForSingleObject(handle, INFINITE);
    return 0;
}
