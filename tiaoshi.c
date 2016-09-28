Breakpoint 0 hit
eax=0c0c0c0c ebx=12c516e4 ecx=133694b0 edx=000009fb esi=12c516f8 edi=12c516e0
eip=76fc08f8 esp=026fd89c ebp=026fd8d8 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000202
ntdll!wtoi+0xf8:
76fc08f8 94              xchg    eax,esp
0:005> p
eax=026fd89c ebx=12c516e4 ecx=133694b0 edx=000009fb esi=12c516f8 edi=12c516e0
eip=76fc08f9 esp=0c0c0c0c ebp=026fd8d8 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000202
ntdll!wtoi+0xf9:
76fc08f9 c3              ret
0:005> p
eax=026fd89c ebx=12c516e4 ecx=133694b0 edx=000009fb esi=12c516f8 edi=12c516e0
eip=76fb13bc esp=0c0c0c10 ebp=026fd8d8 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000202
ntdll+0x13bc:
76fb13bc c3              ret
0:005> p
eax=026fd89c ebx=12c516e4 ecx=133694b0 edx=000009fb esi=12c516f8 edi=12c516e0
eip=76fb94d1 esp=0c0c0c14 ebp=026fd8d8 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000202
ntdll!RtlLockMemoryBlockLookaside+0x8c:
76fb94d1 5b              pop     ebx
0:005> p
eax=026fd89c ebx=76fc08f8 ecx=133694b0 edx=000009fb esi=12c516f8 edi=12c516e0
eip=76fb94d2 esp=0c0c0c18 ebp=026fd8d8 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000202
ntdll!RtlLockMemoryBlockLookaside+0x8d:
76fb94d2 c3              ret
0:005> p
eax=026fd89c ebx=76fc08f8 ecx=133694b0 edx=000009fb esi=12c516f8 edi=12c516e0
eip=76ff5360 esp=0c0c0c1c ebp=026fd8d8 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000202
ntdll!ZwProtectVirtualMemory:
76ff5360 b8d7000000      mov     eax,0D7h
0:005> g
(cd8.860): Access violation - code c0000005 (first chance)
First chance exceptions are reported before any exception handling.
This exception may be expected and handled.
eax=c0000045 ebx=76fc08f8 ecx=0c0c0c18 edx=76ff64f4 esi=12c516f8 edi=12c516e0
eip=0c0c0c40 esp=0c0c0c34 ebp=026fd8d8 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00010202
0c0c0c40 fc              cld
0:005> p
eax=c0000045 ebx=76fc08f8 ecx=0c0c0c18 edx=76ff64f4 esi=12c516f8 edi=12c516e0
eip=76ff6449 esp=0c0c092c ebp=026fd8d8 iopl=0         nv up ei pl nz na po nc
cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00000202
ntdll!KiUserExceptionDispatcher+0x1:
76ff6449 8b4c2404        mov     ecx,dword ptr [esp+4] ss:0023:0c0c0930=0c0c0950
