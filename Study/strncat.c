#include<stdio.h>
#include <string.h>

main()
{
	char d[20]="Golden Global";
	char *s=" View WinIDE Library";
        
	strncat(d,s,5);
	printf("%s",d);

	return 0;
}
//Golden Global View
