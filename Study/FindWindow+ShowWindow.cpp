#include <windows.h>

int main(int argc, char* argv[])
{
	HWND hwnd = ::FindWindow("notepad",NULL);
	if(hwnd)
	{
		  ::ShowWindow(hwnd,SW_HIDE);
	}
	return 0;
}
