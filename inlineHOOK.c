#include <ntifs.h> 
#include <windef.h> 
ULONG g_KiInsertQueueApc; 
ULONG g_uCr0; 
BYTE g_HookCode[5] = { 0xe9, 0, 0, 0, 0 }; //JMP NEAR 
BYTE g_OrigCode[5] = { 0 }; // 原函数的前字节内容 
BYTE jmp_orig_code[7] = { 0xEA, 0, 0, 0, 0, 0x08, 0x00 }; //JMP FAR 
BOOL g_bHooked = FALSE; 
VOID
fake_KiInsertQueueApc ( 
            PKAPC Apc, 
            KPRIORITY Increment 
            ); 
VOID
Proxy_KiInsertQueueApc ( 
            PKAPC Apc, 
            KPRIORITY Increment 
            ); 
void WPOFF() 
{ 
  ULONG uAttr; 
  _asm 
  { 
    push eax; 
    mov eax, cr0; 
    mov uAttr, eax; 
    and eax, 0FFFEFFFFh; // CR0 16 BIT = 0 
    mov cr0, eax; 
    pop eax; 
    cli 
  }; 
  g_uCr0 = uAttr; //保存原有的 CRO 屬性 
} 
VOID WPON() 
{ 
  _asm 
  { 
    sti 
      push eax; 
    mov eax, g_uCr0; //恢復原有 CR0 屬性 
    mov cr0, eax; 
    pop eax; 
  }; 
} 
// 
// 停止inline hook 
// 
VOID UnHookKiInsertQueueApc () 
{ 
  KIRQL oldIrql; 
  WPOFF(); 
  oldIrql = KeRaiseIrqlToDpcLevel(); 
  RtlCopyMemory ( (BYTE*)g_KiInsertQueueApc, g_OrigCode, 5 ); 
  KeLowerIrql(oldIrql); 
  WPON(); 
  g_bHooked = FALSE; 
} 
// 
// 开始inline hook -- KiInsertQueueApc 
// 
VOID HookKiInsertQueueApc () 
{ 
  KIRQL oldIrql; 
  if (g_KiInsertQueueApc == 0) { 
    DbgPrint("KiInsertQueueApc == NULL\n"); 
    return; 
  } 
  //DbgPrint("开始inline hook -- KiInsertQueueApc\n"); 
  DbgPrint( "KiInsertQueueApc的地址t0x%08x\n", (ULONG)g_KiInsertQueueApc ); 
  DbgPrint( "fake_KiInsertQueueApc的地址t0x%08x\n", (ULONG)fake_KiInsertQueueApc ); 
    
  // 保存原函数的前字节内容 
  RtlCopyMemory (g_OrigCode, (BYTE*)g_KiInsertQueueApc, 5); 
  //jmp指令，此处为短跳，计算相对偏移，同时，jmp xxxxxx这条指令占了5个字节 
  *( (ULONG*)(g_HookCode + 1) ) = (ULONG)fake_KiInsertQueueApc - (ULONG)g_KiInsertQueueApc - 5; 
  // 禁止系统写保护，提升IRQL到DPC 
  WPOFF(); 
  oldIrql = KeRaiseIrqlToDpcLevel(); 
  RtlCopyMemory ( (BYTE*)g_KiInsertQueueApc, g_HookCode, 5 ); 
  *( (ULONG*)(jmp_orig_code + 1) ) = (ULONG) ( (BYTE*)g_KiInsertQueueApc + 5 ); 
  RtlCopyMemory ( (BYTE*)Proxy_KiInsertQueueApc, g_OrigCode, 5); 
  RtlCopyMemory ( (BYTE*)Proxy_KiInsertQueueApc + 5, jmp_orig_code, 7); 
  // 恢复写保护，降低IRQL 
  KeLowerIrql(oldIrql); 
  WPON(); 
  g_bHooked = TRUE; 
} 
// 
// 跳转到我们的函数里面进行预处理，裸函数，有调用者进行堆栈的平衡 
// 
__declspec (naked) 
VOID
fake_KiInsertQueueApc ( 
            PKAPC Apc, 
            KPRIORITY Increment 
            ) 
{ 
  // 去掉DbgPrint,不然这个hook会产生递归 
  //DbgPrint("inline hook -- KiInsertQueueApc 成功\n"); 
  __asm 
  { 
    jmp Proxy_KiInsertQueueApc 
  } 
} 
// 
// 代理函数，负责跳转到原函数中继续执行 
// 
__declspec (naked) 
VOID
Proxy_KiInsertQueueApc ( 
            PKAPC Apc, 
            KPRIORITY Increment 
            ) 
{ 
  __asm { // 共字节 
    _emit 0x90 
      _emit 0x90 
      _emit 0x90 
      _emit 0x90 
      _emit 0x90 // 前字节实现原函数的头字节功能 
      _emit 0x90 // 这个填充jmp 
      _emit 0x90 
      _emit 0x90 
      _emit 0x90 
      _emit 0x90 // 这字节保存原函数+5处的地址 
      _emit 0x90  
      _emit 0x90 // 因为是长转移,所以必须是0x0080 
  } 
} 
ULONG GetFunctionAddr( IN PCWSTR FunctionName) 
{ 
  UNICODE_STRING UniCodeFunctionName; 
  RtlInitUnicodeString( &UniCodeFunctionName, FunctionName ); 
  return (ULONG)MmGetSystemRoutineAddress( &UniCodeFunctionName );  
} 
//根据特征值，从KeInsertQueueApc搜索中搜索KiInsertQueueApc 
ULONG FindKiInsertQueueApcAddress() 
{ 
  char * Addr_KeInsertQueueApc = 0; 
  int i = 0; 
  char Findcode[] = { 0xE8, 0xcc, 0x29, 0x00, 0x00 }; 
  ULONG Addr_KiInsertQueueApc = 0; 
  Addr_KeInsertQueueApc = (char *) GetFunctionAddr(L"KeInsertQueueApc"); 
  for(i = 0; i < 100; i ++) 
  { 
    if( Addr_KeInsertQueueApc[i] == Findcode[0] && 
      Addr_KeInsertQueueApc[i + 1] == Findcode[1] && 
      Addr_KeInsertQueueApc[i + 2] == Findcode[2] && 
      Addr_KeInsertQueueApc[i + 3] == Findcode[3] && 
      Addr_KeInsertQueueApc[i + 4] == Findcode[4] 
    ) 
    { 
      Addr_KiInsertQueueApc = (ULONG)&Addr_KeInsertQueueApc[i] + 0x29cc + 5; 
      break; 
    } 
  } 
  return Addr_KiInsertQueueApc; 
} 
VOID OnUnload( IN PDRIVER_OBJECT DriverObject ) 
{ 
  DbgPrint("My Driver Unloaded!"); 
  UnHookKiInsertQueueApc(); 
} 
NTSTATUS DriverEntry( IN PDRIVER_OBJECT theDriverObject, IN PUNICODE_STRING theRegistryPath ) 
{ 
  DbgPrint("My Driver Loaded!"); 
  theDriverObject->DriverUnload = OnUnload; 
  g_KiInsertQueueApc = FindKiInsertQueueApcAddress(); 
  HookKiInsertQueueApc(); 
  return STATUS_SUCCESS; 
}
