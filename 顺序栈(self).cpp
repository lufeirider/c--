#include <stdio.h>
#include<iostream.h>
typedef struct stack
{
	int* top;
	int* base;
	int stackSize;
}stack;

void InitStack(stack &s)
{
	//int numChar[10];
	//s.base = numChar; 不能用栈，因为数据容易被覆盖。

	s.base = new int[10];
	s.top = s.base;
	s.stackSize = 10;
}

void PushStack(stack &s,int &num)
{
	if(s.top-s.base==s.stackSize) //栈满
		return;

	*(s.top++) = num;
	printf("PushStack s.top = %x\n",s.top);
}

void PopStack(stack &s,int &num)
{
	if(s.base == s.top)  //栈空
		return;
	printf("PopStack s.top = %x\n",s.top);
	num = *(--s.top);						//先减是因为pushstack的时候把栈顶提高了。
}

void test()
{
	//temp = 176;
}

int main()
{
	stack s;
	int temp=0;
	InitStack(s);
	printf("s.stackSize = %d\n",s.stackSize);
	int num1=10;
	PushStack(s,num1);
	int num2=11;
	PushStack(s,num2);


	PopStack(s,temp);
	printf("temp = %d\n",temp);
	PopStack(s,temp);
	printf("temp = %d\n",temp);


	return 0;
}

