#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
char mybuf[] = "ww0830";
int main (int argc, char *argv[])
{
	HANDLE hHeap;
	char *buf1, *buf2;
	//我们自己建立一个HEAP
	hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0x10000, 0xfffff);
printf("mybuf addr = %p\n",mybuf);
	//动态分配buf1
buf1 = HeapAlloc(hHeap, 0, 200);
	strcpy(buf1,mybuf);
	printf("buf1 = %s\n",buf1);
	//动态分配buf2
	buf2 = HeapAlloc(hHeap, 0, 16);

	HeapFree(hHeap, 0, buf1);
	HeapFree(hHeap, 0, buf2);
	return 0;
}
