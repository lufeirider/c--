
//修改 IAT 实现 HOOK-API

#include <windows.h>
#include <imagehlp.h>//ImageDirectoryEntryToData
#pragma comment(lib,"imagehlp.lib")
/************************************************************/
char *szModName = NULL ;
char *szHacked = "成功 HOOK-API！" ;
DWORD dwHookFun ;
DWORD dwHookApiAddr;
PIMAGE_IMPORT_DESCRIPTOR pImportDesc ;
PIMAGE_THUNK_DATA32 pThunk;
ULONG uSize ;
/************************************************************/
void MYhook()
{
    __asm
    {
        mov  esp, ebp
        push szHacked
        pop  DWORD PTR [ebp+12]
        pop  ebp
        jmp dwHookApiAddr
    }
}
int main()
{
    HMODULE hInstance = GetModuleHandle(NULL);
    dwHookFun = (DWORD)MYhook;
    dwHookApiAddr = (DWORD)GetProcAddress(LoadLibrary("USER32.dll"), "MessageBoxA") ;
    //通过函数)ImageDirectoryEntryToData获得IAT
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hInstance,
                  TRUE,
                  IMAGE_DIRECTORY_ENTRY_IMPORT,
                  &uSize) ;
    //找到要HOOK的函数所在的模块
    while(pImportDesc->Name)
    {
        szModName = (char *)((PBYTE)hInstance + pImportDesc->Name) ;
        if (strcmp(szModName, "USER32.dll") == 0)
            break ;
        pImportDesc++ ;
    }
    //原始的THUNK信息指针
    pThunk = (PIMAGE_THUNK_DATA32)((PBYTE)hInstance + pImportDesc->FirstThunk) ;
    for(; pThunk->u1.Function; pThunk++)
    {
        if (pThunk->u1.Function == dwHookApiAddr)
        {
            VirtualProtect(&pThunk->u1.Function, 4096, PAGE_READWRITE, 0);
            pThunk->u1.Function = (DWORD)dwHookFun;
            break ;
        }
    }
    //要hook下面这个API
    MessageBoxA(0, "这是正常的!", "xicao", 0);
    return 0 ;
}
