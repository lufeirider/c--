#include <windows.h>
#include <stdio.h>
#include "psapi.h"
#pragma   comment(lib,"psapi.lib")
DWORD processtopid(char *processname)
{
         DWORD    lpidprocesses[1024],cbneeded,cprocesses;
         HANDLE   hprocess;
         HMODULE  hmodule;
         UINT     i;
         TCHAR    normalname[MAX_PATH]=("UnknownProcess");
   
         if(!EnumProcesses(lpidprocesses,sizeof(lpidprocesses),&cbneeded))
         {
                   return -1; 
         }
         cprocesses=cbneeded/sizeof(DWORD);
         for(i=0;i<cprocesses;i++)
         {
                   hprocess=OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE,lpidprocesses[i]);
                   if(hprocess)
                   {
                            if(EnumProcessModules(hprocess,&hmodule,sizeof(hmodule),&cbneeded))
                            {
                                     GetModuleBaseName(hprocess,hmodule,normalname,sizeof(normalname));
                                     if(!strcmp(normalname,processname)) 
                                     {
                                               CloseHandle(hprocess);
											   printf("%d",lpidprocesses[i]);
                                               return (lpidprocesses[i]);
                                     }
                            }
                   }
         }
         CloseHandle(hprocess);
         return 0;
}

int main()
{
	processtopid("notepad++.exe");
	return 0;
}
