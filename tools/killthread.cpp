#include <stdio.h>  
#include <windows.h>  
#include <TlHelp32.h>  

bool EnableDebugPrivilege()   
{   
    HANDLE hToken;   
    LUID sedebugnameValue;   
    TOKEN_PRIVILEGES tkp;   
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {   
        return   FALSE;   
    }   
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))  
    {   
        CloseHandle(hToken);   
        return false;   
    }   
    tkp.PrivilegeCount = 1;   
    tkp.Privileges[0].Luid = sedebugnameValue;   
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;   
    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) 
    {   
        CloseHandle(hToken);   
        return false;   
    }   
    return true;   
}

int main()  
{  
	EnableDebugPrivilege();

    // 为进程的所有线程拍个快照  
    HANDLE hSnapshort = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);  
    if( hSnapshort==INVALID_HANDLE_VALUE )  
    {  
        printf("CreateToolhelp32Snapshot调用失败！\n");  
        return -1;  
    }  
  
    // 获得线程列表，里面记录了线程的详细信息，再使用Thread32First和Thread32Next遍历快照中记录的每个线程信息  
    THREADENTRY32 stcThreadInfo;  
    stcThreadInfo.dwSize = sizeof(stcThreadInfo);  
  
    BOOL  bRet = Thread32First(hSnapshort, &stcThreadInfo);   
	HANDLE ht = NULL;

	DWORD pid = 0;
	printf("pid = ");
	scanf("%d",&pid);
    while (bRet)  
    {  

		if(pid==stcThreadInfo.th32OwnerProcessID)
		{
			printf("线程TID\t\t线程所属进程PID\t\t线程优先级\n");  
			printf("  %d\t\t\t%d\t\t\t %d\n",stcThreadInfo.th32ThreadID, stcThreadInfo.th32OwnerProcessID, stcThreadInfo.tpBasePri);
			ht = OpenThread(PROCESS_ALL_ACCESS,false,stcThreadInfo.th32ThreadID);
			//SuspendThread(ht);
			TerminateThread(ht,0);
		}
        bRet = Thread32Next(hSnapshort, &stcThreadInfo);  
		
    }  
  
    CloseHandle(hSnapshort);  
  
    system("pause");  
    return 0;  
}  
