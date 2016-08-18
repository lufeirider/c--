#include <string.h>
#include <stdio.h>
int main()
{
	int i=0;
	char *p;
	char test[]="qq123456.php";
	p = strstr(test,"php");
	printf("%s",p);
	return 0;
}
