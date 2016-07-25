#include <windows.h>
#include <winbase.h> 
typedef void (*MYPROC)(LPTSTR);		//定义函数指针
int main()
{
	HINSTANCE LibHandle;
	MYPROC ProcAdd;
	LibHandle = LoadLibrary("msvcrt");
	ProcAdd = (MYPROC) GetProcAddress(LibHandle, "system");	//查找system函数地址
	(ProcAdd) ("command.com");		//其实就是执行system("command.com")
	return 0;
}
