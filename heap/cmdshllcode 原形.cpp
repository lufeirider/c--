#include <windows.h>
#include <winbase.h> 
typedef void (*MYPROC)(LPTSTR);		//���庯��ָ��
int main()
{
	HINSTANCE LibHandle;
	MYPROC ProcAdd;
	LibHandle = LoadLibrary("msvcrt");
	ProcAdd = (MYPROC) GetProcAddress(LibHandle, "system");	//����system������ַ
	(ProcAdd) ("command.com");		//��ʵ����ִ��system("command.com")
	return 0;
}
