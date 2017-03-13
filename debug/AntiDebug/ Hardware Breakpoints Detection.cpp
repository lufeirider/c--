#include "Shlwapi.h"
#include "tlhelp32.h"
#include "Windows.h"
#include "eh.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static DWORD NewEip;
static DWORD lpOldHandler;
typedef LPTOP_LEVEL_EXCEPTION_FILTER (_stdcall  *pSetUnhandledExceptionFilter)(
                      LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter 
                      );
pSetUnhandledExceptionFilter lpSetUnhandledExceptionFilter;

static bool isDebuggedHBP=0;
LONG WINAPI TopUnhandledExceptionFilterHBP(
	struct _EXCEPTION_POINTERS *ExceptionInfo
)
{
	_asm pushad
	AfxMessageBox("回调函数被调用");
	ExceptionInfo->ContextRecord->Eip=NewEip;
	if(0!=ExceptionInfo->ContextRecord->Dr0||0!=ExceptionInfo->ContextRecord->Dr1||0!=ExceptionInfo->ContextRecord->Dr2||0!=ExceptionInfo->ContextRecord->Dr3)
		isDebuggedHBP=1; //检测有无硬件断点
	ExceptionInfo->ContextRecord->Dr0=0; //禁用硬件断点，置 0
	ExceptionInfo->ContextRecord->Dr1=0;
	ExceptionInfo->ContextRecord->Dr2=0;
	ExceptionInfo->ContextRecord->Dr3=0;
	ExceptionInfo->ContextRecord->Dr6=0;
	ExceptionInfo->ContextRecord->Dr7=0;
	ExceptionInfo->ContextRecord->Eip=NewEip; //转移到安全位置
	_asm popad
	return EXCEPTION_CONTINUE_EXECUTION;
}

void CMfctestDlg::OnOK() 
{
	lpSetUnhandledExceptionFilter  = (pSetUnhandledExceptionFilter)GetProcAddress(LoadLibrary(("kernel32.dll")),"SetUnhandledExceptionFilter");
	lpOldHandler=(DWORD)lpSetUnhandledExceptionFilter(TopUnhandledExceptionFilterHBP);
	_asm{
		mov NewEip,offset safe //方式二，更简单
		int 3
		mov isDebuggedHBP,1 //调试时可能也不会触发异常去检测硬件断点
	safe:
	} 
	if(1==isDebuggedHBP){
		AfxMessageBox("发现 OD");
	}else{
		AfxMessageBox("没有 OD");
	}
}
