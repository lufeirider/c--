eax=00000000 ebx=093806b0 ecx=092b0000 edx=00000000 esi=08c69f90 edi=00008282
eip=275c87c1 esp=00127ad0 ebp=00127ae0 iopl=0         nv up ei pl zr na pe nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000246
MSCOMCTL!DllGetClassObject+0x41a7d:
275c87c1 8bcf            mov     ecx,edi
0:000> r edi=0x2c
0:000> u
MSCOMCTL!DllGetClassObject+0x41a7d:
275c87c1 8bcf            mov     ecx,edi
275c87c3 8b7d08          mov     edi,dword ptr [ebp+8]
275c87c6 8bc1            mov     eax,ecx
275c87c8 c1e902          shr     ecx,2
275c87cb f3a5            rep movs dword ptr es:[edi],dword ptr [esi]
275c87cd 8bc8            mov     ecx,eax
275c87cf 8b4510          mov     eax,dword ptr [ebp+10h]
275c87d2 83e103          and     ecx,3
0:000> p
eax=00000000 ebx=093806b0 ecx=0000002c edx=00000000 esi=08c69f90 edi=0000002c
eip=275c87c3 esp=00127ad0 ebp=00127ae0 iopl=0         nv up ei pl zr na pe nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000246
MSCOMCTL!DllGetClassObject+0x41a7f:
275c87c3 8b7d08          mov     edi,dword ptr [ebp+8] ss:0023:00127ae8=00127b0c
0:000> p
eax=00000000 ebx=093806b0 ecx=0000002c edx=00000000 esi=08c69f90 edi=00127b0c
eip=275c87c6 esp=00127ad0 ebp=00127ae0 iopl=0         nv up ei pl zr na pe nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000246
MSCOMCTL!DllGetClassObject+0x41a82:
275c87c6 8bc1            mov     eax,ecx
0:000> p
eax=0000002c ebx=093806b0 ecx=0000002c edx=00000000 esi=08c69f90 edi=00127b0c
eip=275c87c8 esp=00127ad0 ebp=00127ae0 iopl=0         nv up ei pl zr na pe nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000246
MSCOMCTL!DllGetClassObject+0x41a84:
275c87c8 c1e902          shr     ecx,2
0:000> p
eax=0000002c ebx=093806b0 ecx=0000000b edx=00000000 esi=08c69f90 edi=00127b0c
eip=275c87cb esp=00127ad0 ebp=00127ae0 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000202
MSCOMCTL!DllGetClassObject+0x41a87:
275c87cb f3a5            rep movs dword ptr es:[edi],dword ptr [esi]
0:000> dds esp
00127ad0  00000000
00127ad4  08c616d4 <Unloaded_i.dll>+0x8c616d3
00127ad8  093806b0 <Unloaded_i.dll>+0x93806af
00127adc  00008282 <Unloaded_i.dll>+0x8281
00127ae0  00127b14 <Unloaded_i.dll>+0x127b13
00127ae4  275c8a0a MSCOMCTL!DllGetClassObject+0x41cc6
00127ae8  00127b0c <Unloaded_i.dll>+0x127b0b
00127aec  08c69f90 <Unloaded_i.dll>+0x8c69f8f
00127af0  00008282 <Unloaded_i.dll>+0x8281
00127af4  00000000
00127af8  08c616d4 <Unloaded_i.dll>+0x8c616d3
00127afc  093806b0 <Unloaded_i.dll>+0x93806af
00127b00  6a626f43
00127b04  00000064 <Unloaded_i.dll>+0x63
00127b08  00008282 <Unloaded_i.dll>+0x8281
00127b0c  08c617c0 <Unloaded_i.dll>+0x8c617bf
00127b10  275859e4 MSCOMCTL!DllCanUnloadNow+0x2a31
00127b14  00127b3c <Unloaded_i.dll>+0x127b3b
00127b18  275e701a MSCOMCTL!DLLGetDocumentation+0xd08
00127b1c  08c616d4 <Unloaded_i.dll>+0x8c616d3
00127b20  093806b0 <Unloaded_i.dll>+0x93806af
00127b24  00000000
00127b28  08c616b0 <Unloaded_i.dll>+0x8c616af
00127b2c  08c69268 <Unloaded_i.dll>+0x8c69267
00127b30  275ac296 MSCOMCTL!DllGetClassObject+0x25552
00127b34  00000001 <Unloaded_i.dll>
00127b38  00127b5c <Unloaded_i.dll>+0x127b5b
00127b3c  00127b5c <Unloaded_i.dll>+0x127b5b
00127b40  275e7361 MSCOMCTL!DLLGetDocumentation+0x104f
00127b44  08c616d4 <Unloaded_i.dll>+0x8c616d3
00127b48  093806b0 <Unloaded_i.dll>+0x93806af
00127b4c  093806b0 <Unloaded_i.dll>+0x93806af
0:000> p
eax=0000002c ebx=093806b0 ecx=00000000 edx=00000000 esi=08c69fbc edi=00127b38
eip=275c87cd esp=00127ad0 ebp=00127ae0 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000202
MSCOMCTL!DllGetClassObject+0x41a89:
275c87cd 8bc8            mov     ecx,eax
0:000> dds esp
00127ad0  00000000
00127ad4  08c616d4 <Unloaded_i.dll>+0x8c616d3
00127ad8  093806b0 <Unloaded_i.dll>+0x93806af
00127adc  00008282 <Unloaded_i.dll>+0x8281
00127ae0  00127b14 <Unloaded_i.dll>+0x127b13
00127ae4  275c8a0a MSCOMCTL!DllGetClassObject+0x41cc6
00127ae8  00127b0c <Unloaded_i.dll>+0x127b0b
00127aec  08c69f90 <Unloaded_i.dll>+0x8c69f8f
00127af0  00008282 <Unloaded_i.dll>+0x8281
00127af4  00000000
00127af8  08c616d4 <Unloaded_i.dll>+0x8c616d3
00127afc  093806b0 <Unloaded_i.dll>+0x93806af
00127b00  6a626f43
00127b04  00000064 <Unloaded_i.dll>+0x63
00127b08  00008282 <Unloaded_i.dll>+0x8281
00127b0c  00000000
00127b10  00000000
00127b14  00000000
00127b18  7ffa4512
00127b1c  90909090
00127b20  90909090
00127b24  3350ec83 <Unloaded_i.dll>+0x3350ec82
00127b28  696853db
00127b2c  6820676e
00127b30  6e726157
00127b34  6853c48b
00127b38  00127b5c <Unloaded_i.dll>+0x127b5b
00127b3c  00127b5c <Unloaded_i.dll>+0x127b5b
00127b40  275e7361 MSCOMCTL!DLLGetDocumentation+0x104f
00127b44  08c616d4 <Unloaded_i.dll>+0x8c616d3
00127b48  093806b0 <Unloaded_i.dll>+0x93806af
00127b4c  093806b0 <Unloaded_i.dll>+0x93806af
