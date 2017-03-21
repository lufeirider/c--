#include<windows.h>
#include<UrlMon.h>
#pragma comment(lib,"urlmon.lib")
int main()
{
	char strPath[] = "c:\\test.exe";
	
	URLDownloadToCacheFile(NULL,"file://c:\\windows\\notepad.exe",strPath,MAX_PATH,0,NULL);
    ShellExecute(0,"open",strPath,NULL,NULL,SW_SHOW);

	return 0;
}
