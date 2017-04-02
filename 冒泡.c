#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

long sumTwoSmallestNumbers(std::vector<int> numbers)
{
	int temp;
	for (int i = 0; i < numbers.size() - 1; i++)
	{
		for (int j = 0; j < numbers.size() - i -1; j++)
		{
			if (numbers[j] > numbers[j + 1])
			{
				temp = numbers[j];
				numbers[j] = numbers[j + 1];
				numbers[j + 1] = temp;
			}
		}
	}
	for (size_t i = 0; i < numbers.size(); i++)
	{
		cout << numbers[i] << endl;
	}
	return 0;
}

int main()
{
	long actual = sumTwoSmallestNumbers({ 22 ,5, 8, 12, 19,1 });
	getchar();
	return 0;
}
