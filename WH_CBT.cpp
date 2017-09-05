#include <windows.h>
#include <stdio.h>
HHOOK		hHook = NULL;
HINSTANCE	hDll = NULL;
char		Buff[10] = {0};


LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char szBuff[255] = {0};
	memset(szBuff,0,255);
    static HWND g_hWndDialog = NULL;
    switch (nCode)
    {
        case HCBT_CREATEWND:
        {
            HWND hWnd = (HWND)wParam;
            LPCBT_CREATEWND pcbt = (LPCBT_CREATEWND)lParam;
            LPCREATESTRUCT pcs = pcbt->lpcs;
			
			//OutputDebugString(pcs->lpszName);
			sprintf(szBuff,"hand = %x",pcs->hwndParent);
			//strcpy(szBuff,pcs->hwndParent);
			OutputDebugString(szBuff);
			

            if ((DWORD)(pcs->dwExStyle) == 0x40310)
            {
                MessageBox(NULL, "Shell open!", "Shell open!", 0);
                g_hWndDialog = hWnd;
            }
            break;
        }
        default:
            break;
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

BOOL StartHook(DWORD dwThreadId)
{
	hHook = SetWindowsHookEx(WH_CBT, CBTProc, hDll, 0);
	//W = Windows  H = Hook
	if (hHook == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	hDll = (HINSTANCE)hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
