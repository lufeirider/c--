#include <windows.h>
#include <stdio.h>
int main()
{
	HANDLE isWait=OpenEvent(EVENT_MODIFY_STATE, FALSE, "test");
	SetEvent(isWait);
	getchar();
	return 0;
}
