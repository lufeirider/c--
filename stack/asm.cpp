int main()
{
	__asm
	{
// ==== 将所要是用的函数的hash值入栈 ====
		cld							//清除标志位DF
		push	0x1e380a6a			//MessageBoxA的hash值
		push	0x4fd18963			//ExitProcess的hash值
		push	0x0c917432			//LoadLibraryA的hash值
		mov		esi,esp				//esi保存的是栈顶第一个函数，即LoadLibraryA的hash值
		lea		edi,[esi-0xc]
// ==== 开辟一些桟空间 ====
		xor		ebx,ebx				//将ebx清零
		mov		bh,0x04				//此时ebx为0x400
		sub		esp,ebx				//开辟0x400大小的空间
// ==== 压入"user32.dll" ====
		mov		bx,0x3233
		push	ebx					//压入字符"32"
		push 	0x72657375			//压入字符"user"
		push	esp
		xor 	edx,edx				//将edx清零
// ==== 查找kernel32.dll的基地址 ====
		mov		ebx,fs:[edx+0x30]  // [TEB+0x30]是PEB的位置
		mov		ecx,[ebx+0xC]    // [PEB+0xC]是PEB_LDR_DATA的位置
		mov		ecx,[ecx+0x1C]   // [PEB_LDR_DATA+0x1C]是InInitializationOrderModuleList的位置
		mov		ecx,[ecx]        // 进入链表第一个就是ntdll.dll
		mov		ebp,[ecx+0x8]     // ebp保存的是kernel32.dll的基地址
		
	find_lib_funtions:
		lodsd
		cmp		eax,0x1e380a6a		//与MessageBoxA的hash值进行比较
		jne		find_functions		//如果不相等，则继续查找
		xchg	eax,ebp
		call	[edi-0x8]
		xchg	eax,ebp

// ==== 在PE文件中查找相应的API函数 ====   
	find_functions:
		pushad                      // 保护所有寄存器中的内容
		mov  eax,[ebp+0x3C]  // PE头
		mov  ecx,[ebp+eax+0x78] // 导出表的指针
		add  ecx,ebp    
		mov  ebx,[ecx+0x20]  // 导出函数的名字列表
		add  ebx,ebp             
		xor  edi,edi             // 清空edi中的内容，用作索引
// ==== 循环读取导出表函数 ====   
	next_function_loop:
		inc  edi                 // edi不断自增，作为索引
		mov     esi,[ebx+edi*4]     // 从列表数组中读取
		add  esi,ebp       // esi保存的是函数名称所在地址
		cdq                         // 把edx的每一位置成eax的最高位，再把edx扩展为eax的高位，即变为64位	

// ==== hash值的计算 ====
	hash_loop:
		movsx eax,byte ptr[esi]   // 每次取出一个字符放入eax中
		cmp  al,ah            // 验证eax是否为0x0，即结束符
		jz  compare_hash    // 如果上述结果为零，说明hash值计算完毕，则进行hash值的比较
		ror     edx,7               // 如果cmp的结果不为零，则进行循环右移7位的操作
		add  edx,eax             // 将循环右移的值不断累加
		inc  esi                 // esi自增，用于读取下一个字符
		jmp  hash_loop           // 跳到hash_loop的位置继续计算
// ==== hash值的比较 ====
	compare_hash:
		cmp 	edx,[esp+0x1c]
		jnz		next_function_loop
		mov		ebx,[ecx+0x24]
		add		ebx,ebp
		mov		di,[ebx+2*edi]
		mov		ebx,[ecx+0x1c]
		add		ebx,ebp
		add		ebp,[ebx+4*edi]
		xchg	eax,ebp
		pop		edi
		stosd
		push	edi
		popad
		cmp		eax,0x1e380a6a
		jne		find_lib_funtions
		
// ==== hash值的比较 ====
	funtion_call:
	sub esp,0x50
	xor ebx,ebx
	push ebx     // cut string
	push 0x20676e69   
	push 0x6e726157    // push "Warning"
	mov eax,esp
	push ebx             // cut string  
	push 0x2020292e
	push 0x592e4a20
	push 0x79622821
	push 0x64656b63
	push 0x6168206e
	push 0x65656220
	push 0x65766168
	push 0x20756f59    // push "You have been hacked!(by J.Y.)"
	mov ecx,esp
	push ebx
	push eax
	push ecx
	push ebx
	call [edi-0x04]           // call MessageBox
	push ebx
	call [edi-0x08]            // call ExitProcess
	}
}