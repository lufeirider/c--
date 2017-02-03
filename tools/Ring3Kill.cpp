#include <windows.h>
#include <Ntsecapi.h>
#include <Aclapi.h>
#include <tlhelp32.h>
#include <ntdll.h>

//////////////////////////////////////////////////////////////////////////

#pragma comment (lib,"ntdll.lib")
#pragma comment (lib, "Kernel32.lib")
#pragma comment (lib, "Advapi32.lib")
//减少体积
#pragma comment(lib,"msvcrt.lib")
#pragma comment(linker, "/align:512")
//#pragma comment (linker, "/subsystem:windows")	//则表示该应用程序运行时不需要控制台
//#pragma comment (linker, "/ENTRY:main")			//则表示入口点是main

//////////////////////////////////////////////////////////////////////////

typedef struct _SYSTEM_HANDLE_INFORMATION  
{  
    ULONG			ProcessId;				//进程的标识ID   
    UCHAR			ObjectTypeNumber;		//对象类型 
    UCHAR			Flags;					//0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT
    USHORT			Handle;					//对象句柄的数值
    PVOID			Object;					//对象句柄所指的内核对象地址
    ACCESS_MASK		rantedAccess;			//创建句柄时所准许的对象的访问权 
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;


DWORD GetPidByName (char *szName);
void KillProcess (ULONG dwProcessId);
BOOL EnablePrivilege(HANDLE hToken, LPCTSTR szPrivName, BOOL fEnable);


//////////////////////////////////////////////////////////////////////////
// 主函数入口
//
void main()
{
    ULONG Pid;
    HANDLE hToken;

    OpenProcessToken (GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
    EnablePrivilege (hToken, SE_DEBUG_NAME, TRUE);

    if (Pid = GetPidByName("notepad.exe"))
    {
        KillProcess (Pid);
    }
    else
    {
        //MessageBox (NULL, TEXT("The process is not exit,please check out."), TEXT("!"), MB_OK);
    }
}

/////////////////////////////////////////////////////////////////////////
//-----------------------------------
// 函数名: GetPidByName
//
// 参数: char *szName
// --> 进程名
//
// 返回值: 指定进程的PID
//
// 功能: 通过CreateToolhelp32Snapshot函数遍历进程,找到制定进程的PID
// 对RootKit基本无用
//
DWORD GetPidByName (char *szName)
{
    HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 pe32 = {0};
    DWORD dwRet = 0;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
        return 0;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First (hProcessSnap, &pe32))
    {
        do
        {
            if (lstrcmpi(szName, pe32.szExeFile) == 0)
            {
                dwRet = pe32.th32ProcessID;
                break;
            }
        }
        while (Process32Next(hProcessSnap, &pe32));
    }
    else
    {
        return 0;
    }

    if(hProcessSnap != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hProcessSnap);
    }

    return dwRet;
}


/////////////////////////////////////////////////////////////////////////
//-----------------------------------
// 函数名: KillProcess
//
// 参数: ULONG dwProcessId
// --> 进程ID
//
// 返回值: NULL
//
// 功能: 向指定的进程空间填充垃圾信息.使进程死掉
//
void KillProcess (ULONG dwProcessId)
{
    HMODULE hNTDLL = GetModuleHandle("ntdll.dll");
    HANDLE ph, h_dup;

    PSYSTEM_HANDLE_INFORMATION h_info;
    PROCESS_BASIC_INFORMATION pbi;

    // 得到 csrss.exe 进程的PID
    HANDLE csrss_id = (HANDLE) GetPidByName ("csrss.exe");
    CLIENT_ID client_id;
    client_id.UniqueProcess = csrss_id;
    client_id.UniqueThread = 0;


    // 初始化对象结构体
    OBJECT_ATTRIBUTES attr;
    attr.Length = sizeof(OBJECT_ATTRIBUTES);
    attr.RootDirectory = 0;
    attr.ObjectName = 0;
    attr.Attributes = 0;
    attr.SecurityDescriptor = 0;
    attr.SecurityQualityOfService = 0;


    ////////////////////////////////////////////////////////////////////////////////////
    // 获得这些函数的实际地址


    ////////////////////////////////////////////////////////////////////////////////////


    // 打开CSRSS.EXE，获得其句柄
    ZwOpenProcess (&ph, PROCESS_ALL_ACCESS, &attr, &client_id);
	
    ULONG bytesIO = 0x400000;
    PVOID buf = 0;

    ZwAllocateVirtualMemory (GetCurrentProcess(), &buf, 0, &bytesIO, MEM_COMMIT, PAGE_READWRITE);

    // 为 ZwQuerySystemInformation 函数传递16号参数.获得系统句柄信息保存在buff中
    // buff的开始出保存的是系统句柄的数量.偏移4才是句柄信息
    ZwQuerySystemInformation (SystemHandleInformation, buf, 0x400000, &bytesIO);
    ULONG NumOfHandle = (ULONG) buf;
    h_info = (PSYSTEM_HANDLE_INFORMATION)((ULONG)buf + 4);

    for (ULONG i = 0 ; i < NumOfHandle; i++, h_info++)
    {
		//PROCESS 对应 ObjectTypeNumber 为5
        if ((h_info->ProcessId == (ULONG)csrss_id) && (h_info->ObjectTypeNumber == 5))
        {
            // 复制句柄
            if (NtDuplicateObject(
                        ph,							//原进程句柄
                        (PHANDLE)h_info->Handle,	//需要复制句柄
                        (HANDLE) - 1,				//目的进程句柄
                        &h_dup,						//复制到这个
                        0,
                        0,
                        DUPLICATE_SAME_ACCESS) == STATUS_SUCCESS)
            {

                ZwQueryInformationProcess(h_dup, (enum _PROCESSINFOCLASS)0, &pbi, sizeof(pbi), &bytesIO);
            }


            if (pbi.UniqueProcessId == dwProcessId)
            {
                //MessageBox(0, "目标已确定!", "!", MB_OK);

                for (i = 0x1000; i < 0x80000000; i = i + 0x1000)
                {
                    PVOID pAddress = (PVOID) i;
                    ULONG sz = 0x1000;
                    ULONG oldp;

                    if (ZwProtectVirtualMemory (h_dup, &pAddress, &sz, PAGE_EXECUTE_READWRITE, &oldp) == STATUS_SUCCESS)
                    {
                        ZwWriteVirtualMemory(h_dup, pAddress, buf, 0x1000, &oldp);
                    }
                }

                //MessageBox(0, "任务已完成！", "!", 0);
                // ZwClose(h_dup);
                break;
            }
        }
    }

    bytesIO = 0;
    ZwFreeVirtualMemory(GetCurrentProcess(), &buf, &bytesIO, MEM_RELEASE);
}


/////////////////////////////////////////////////////////////////////////

//-----------------------------------
// 函数名: EnablePrivilege
//
// 参数: HANDLE hToken ---> 进程句柄
// LPCTSTR szPrivName --->
// BOOL fEnable --->
//
// 返回值: TRUE | FALSE
//
// 功能: 提升当前进程到指定的特权级
//
BOOL EnablePrivilege(HANDLE hToken, LPCTSTR szPrivName, BOOL fEnable)
{
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;

    LookupPrivilegeValue (NULL, szPrivName, &tp.Privileges[0]. Luid);

    tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;

    AdjustTokenPrivileges (hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

    return((GetLastError() == ERROR_SUCCESS));
}
