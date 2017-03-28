#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int test()
{
  char* szSource="2.exe";
  char* szDestination="3.exe";

  FILE *hSource;
  FILE *hDestination;
   
  DWORD dwKey=0x6a6a6a6a;
  char* pbBuffer;
  DWORD dwBufferLen=sizeof(DWORD);
  DWORD dwCount;
  DWORD dwData;

  hSource = fopen(szSource,"rb");// 打开源文件.
  hDestination = fopen(szDestination,"wb");    //打开目标文件
  if (hSource==NULL) {printf("open Source File error !"); return false ;}
  if (hDestination==NULL){ printf("open Destination File error !");  return false ;}
   
  //分配缓冲区
  pbBuffer=(char* )malloc(dwBufferLen);
   
  do {
    // 从源文件中读出dwBlockLen个字节
    dwCount = fread(pbBuffer, 1, dwBufferLen, hSource);
    //加密数据
    dwData = *(DWORD*)pbBuffer;  //char* TO dword
    dwData^=dwKey;        //xor operation
    pbBuffer = (char *) &dwData;
    // 将加密过的数据写入目标文件
    fwrite(pbBuffer, 1, dwCount, hDestination);
  } while(!feof(hSource));
  
    //关闭文件、释放内存
     fclose(hSource);
     fclose(hDestination);
   
  printf("%s is encrypted to %s\n",szSource,szDestination);
  return 0;
}

int main(int argc, char *argv[])
{
	test();
	return 0;
}
