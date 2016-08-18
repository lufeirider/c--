#include<fcntl.h>
#include<stdio.h>
main()
{
	int fd,size;
	char s [ ]="Linux Programmer!/n";
	char buffer[80];
	fd=open("/tmp/temp",O_WRONLY|O_CREAT);
	write(fd,s,sizeof(s));
	close(fd);
	
	fd=open("/tmp/temp",O_RDONLY);
	size=read(fd,buffer,sizeof(buffer));
	close(fd);
	printf("%s",buffer);
}
