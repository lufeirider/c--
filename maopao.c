#include <stdio.h>

int main()
{
	int numbers[] = { 0,3,1,4 };
	int temp;
	//printf("%d\n", sizeof(numbers)/sizeof(int));
	for (int i = 0; i < sizeof(numbers) / sizeof(int); i++)
	{
		for (int j = i; j < sizeof(numbers) / sizeof(int); j++)
		{
			if (numbers[i] > numbers[j])
			{
				temp = numbers[i];
				numbers[i] = numbers[j];
				numbers[j] = temp;
			}
		}
	}
	for (int i = 0; i < sizeof(numbers) / sizeof(int); i++)
	{
		printf("%d\n",numbers[i]);
	}
	getchar();
	return 0;
}
