#include<windows.h>
#include<stdio.h>
int main()
{
    char Text[25] = {"abcd222"};
    DWORD dwSize;
    HANDLE FileHandle = CreateFile("c:\\test.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
    SetFilePointer(FileHandle, 0, NULL, FILE_END);
    WriteFile(FileHandle, Text, 7, &dwSize, NULL);
    return 0;
}
