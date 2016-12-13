#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")
#include <vector>
using namespace std; 
int main(int argc, char* argv[])
{
    vector<char> v;
    CHAR szUrl[] = "http://www.baidu.com/";
    CHAR szAgent[] = "";
    HINTERNET hInternet1 = 
        InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
    if (NULL == hInternet1)
     {
        InternetCloseHandle(hInternet1);
        return FALSE;
     }
    HINTERNET hInternet2 = 
        InternetOpenUrl(hInternet1,szUrl,NULL,NULL,INTERNET_FLAG_NO_CACHE_WRITE,NULL);
    if (NULL == hInternet2)
     {
        InternetCloseHandle(hInternet2);
        InternetCloseHandle(hInternet1);
        return FALSE;
     }
    DWORD dwMaxDataLength = 500;
    PBYTE pBuf = (PBYTE)malloc(dwMaxDataLength*sizeof(TCHAR));
    if (NULL == pBuf)
     {
        InternetCloseHandle(hInternet2);
        InternetCloseHandle(hInternet1);
        return FALSE;
     }
    DWORD dwReadDataLength = NULL;
    BOOL bRet = TRUE;
    do 
    {
        ZeroMemory(pBuf,dwMaxDataLength*sizeof(TCHAR));
        bRet = InternetReadFile(hInternet2,pBuf,dwMaxDataLength,&dwReadDataLength);
        for (DWORD dw = 0;dw < dwReadDataLength;dw++)
         {
            v.push_back(pBuf[dw]);
         }
     } while (NULL != dwReadDataLength);
    vector<char>::iterator i;
    for(i=v.begin(); i!=v.end(); i++)
        printf("%c",*i);
    return 0;
} 
