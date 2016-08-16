#include <stdio.h>
#include <windows.h>

int main()
{
	typedef struct student
	{
		int age;
	} stu,*pstu;
	
	stu stu1;
	stu1.age=1;
	printf("%d",stu1.age);
	
	pstu stu2 = (struct student*)malloc(sizeof(struct student));;
	stu2->age=2;
	printf("%d",stu2->age);
	return 0;
}
