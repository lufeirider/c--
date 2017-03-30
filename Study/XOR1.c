#include <stdio.h>
#include <windows.h>

int main()
{
	char test[] = "AAAAAAA";
	char xor[sizeof(test)];				//这里
	xor[sizeof(test)-1] = 0x00;			//要填充00，因为申请一个未定义的数组，他没有填充00

	for(int i=0;i<sizeof(test)-1;i++)
	{
		xor[i] = test[i]^0x01;
	}

	printf("%s\n",test);
	printf("%s\n",xor);
	return 0;
}
