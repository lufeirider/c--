#include <windows.h>

int main(int argc, char* argv[])
{
	HWND hwnd = ::FindWindow("notepad",NULL);
	//HWND hwnd = ::FindWindow("ConsoleWindowClass",NULL);
	//隐藏当前窗口	
	if(hwnd)
	{
		  ::ShowWindow(hwnd,SW_HIDE);
	}
	return 0;
}
