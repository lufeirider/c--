#include <windows.h>
#include <Ntsecapi.h>
#include <Aclapi.h>
#include <tlhelp32.h>
#include <ntdll.h>
#include <stdio.h>


#pragma comment (lib,"ntdll.lib")


//////////////////////////////////////////////////////////////////////////
//函数定义

typedef NTSTATUS (_stdcall *ZWOPENPROCESS)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PCLIENT_ID);
ZWOPENPROCESS myZwOpenProcess;
//////////////////////////////////////////////////////////////////////////


// 主函数入口
//
void main()
{
	//////////////////////////////////////////////////////////////////////////
	//获取真正的地址
	myZwOpenProcess = (ZWOPENPROCESS)GetProcAddress(LoadLibrary(("ntdll.dll")),"ZwOpenProcess");

	//////////////////////////////////////////////////////////////////////////
    HANDLE ph;
	HANDLE csrss_id = (HANDLE)1237;
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


    ZwOpenProcess (&ph, PROCESS_ALL_ACCESS, &attr, &client_id);


}
