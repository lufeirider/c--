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

http://www.cnblogs.com/kkun/archive/2011/11/23/2260280.html
1(当前排序) 1(3和2交换) 1（3和1交换）
2           2           3
3           3           2

1(当前排序) 1(2和3交换)
3           2
2           3
