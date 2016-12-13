#include <windows.h>
#include <stdio.h>
void TestErrorInfo(void)
 {
        
        if (!CreateDirectory(("e://"),0))
        {
              TCHAR szBuf[128];
              LPVOID lpMsgBuf;
              DWORD dw = GetLastError();

             FormatMessage(
                  FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  dw,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0, NULL );

              wsprintf(szBuf,("%s ( %d ): %s"),("CreateDirectory"), dw, lpMsgBuf);
			  printf(szBuf);

              LocalFree(lpMsgBuf);

              
              OutputDebugString(szBuf);
        }
       
 }
int main()
{
	TestErrorInfo();
	return 0;
}
