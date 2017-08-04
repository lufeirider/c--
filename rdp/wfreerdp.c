/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Windows Client
 *
 * Copyright 2009-2011 Jay Sorg
 * Copyright 2010-2011 Vic Lee
 * Copyright 2010-2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/windows.h>

#include <winpr/crt.h>
#include <winpr/credui.h>

#include <freerdp/freerdp.h>
#include <freerdp/constants.h>

#include <freerdp/client/file.h>
#include <freerdp/client/cmdline.h>
#include <freerdp/client/channels.h>
#include <freerdp/channels/channels.h>

#include "resource.h"

#include "wf_client.h"

#include <shellapi.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
	int status;
	HANDLE thread;
	wfContext* wfc;
	DWORD dwExitCode;
	rdpContext* context;
	rdpSettings* settings;
	RDP_CLIENT_ENTRY_POINTS clientEntryPoints;
	int ret = 1;
	int argc = 0, i;
	LPWSTR* args;
	LPWSTR cmd;
	char** argv;




	wchar_t szTemp[255] = { 0 };
	wchar_t szCmd[255] = {0};
	DWORD dwCode;
	DWORD dwFlag = 0;
	

	FILE *fpIp;
	fpIp = fopen("ip.rdp", "r");
	wchar_t szip[1024] = {0};
	if (fpIp == NULL)
	{
		MessageBoxW(NULL, "open ip.rdp file error", L"test", NULL);
		return -1;
	}
	while (fgetws(szip, sizeof(szip), fpIp))
	{
		int len = wcslen(szip);
		if ('\n' == szip[len - 1])
		{
			szip[len - 1] = '\0'; // È¥µô'\n'×Ö·û  
			len--;
		}
		/////////////////////////////////////////////////////////////////
		//MessageBoxW(NULL, szip, L"test", NULL);
		/////////////////////////////////////////////////////////////////

		FILE *fpUserName;
		fpUserName = fopen("username.rdp", "r");
		wchar_t szUserName[1024] = { 0 };
		if (fpUserName == NULL)
		{
			MessageBoxW(NULL, "open username.rdp file error", L"test", NULL);
			return -1;
		}
		while (fgetws(szUserName, sizeof(szUserName), fpUserName))
		{
			if (dwFlag == 1)
			{
				dwFlag = 0;
				break;
			}
			int len = wcslen(szUserName);
			if ('\n' == szUserName[len - 1])
			{
				szUserName[len - 1] = '\0';
				len--;
			}
			/////////////////////////////////////////////////////////////////
			//MessageBoxW(NULL, szUserName, L"test", NULL);
			////////////////////////////////////////////////////////////////

			FILE *fpPassWord;
			fpPassWord = fopen("password.rdp", "r");
			wchar_t szPassWord[1024] = { 0 };
			if (fpPassWord == NULL)
			{
				MessageBoxW(NULL, "open password.rdp file error", L"test", NULL);
				return -1;
			}
			while (fgetws(szPassWord, sizeof(szPassWord), fpPassWord))
			{
				if (dwFlag == 1)
				{
					break;
				}
				int len = wcslen(szPassWord);
				if ('\n' == szPassWord[len - 1])
				{
					szPassWord[len - 1] = '\0';
					len--;
				}
				/////////////////////////////////////////////////////////////////
				//MessageBoxW(NULL, szPassWord, L"test", NULL);
				/////////////////////////////////////////////////////////////////
				ZeroMemory(&clientEntryPoints, sizeof(RDP_CLIENT_ENTRY_POINTS));
				clientEntryPoints.Size = sizeof(RDP_CLIENT_ENTRY_POINTS);
				clientEntryPoints.Version = RDP_CLIENT_INTERFACE_VERSION;
				RdpClientEntry(&clientEntryPoints);
				context = freerdp_client_context_new(&clientEntryPoints);

				if (!context)
					return -1;

				cmd = GetCommandLineW();
				//cmd = L"wfreerdp.exe /u:administrator /p:123456 /v:192.168.62.131:3389";
				//cmd = L"wfreerdp.exe /v:192.168.62.131:3389 /u:administrator /p:123456";

				wcscpy(szCmd, L"wfreerdp.exe ");

				wsprintf(szTemp, L"/v:%ls ", szip);
				wcscat(szCmd, szTemp);

				wsprintf(szTemp,L"/u:%ls ",szUserName);
				wcscat(szCmd, szTemp);

				wsprintf(szTemp, L"/p:%ls", szPassWord);
				wcscat(szCmd, szTemp);

				cmd = szCmd;
				//wprintf(L"%ls", szCmd);


				if (!cmd)
					goto out;

				args = CommandLineToArgvW(cmd, &argc);

				if (!args)
					goto out;

				argv = calloc(argc, sizeof(char*));

				if (!argv)
					goto out;

				for (i = 0; i < argc; i++)
				{
					int size = WideCharToMultiByte(CP_UTF8, 0, args[i], -1, NULL, 0, NULL, NULL);
					argv[i] = calloc(size, sizeof(char));

					if (!argv[i])
						goto out;

					if (WideCharToMultiByte(CP_UTF8, 0, args[i], -1, argv[i], size, NULL,
						NULL) != size)
						goto out;
				}

				settings = context->settings;
				wfc = (wfContext*)context;

				if (!settings || !wfc)
					goto out;

				status = freerdp_client_settings_parse_command_line(settings, argc, argv,
					FALSE);

				if (status)
				{
					freerdp_client_settings_command_line_status_print(settings, status, argc, argv);
					goto out;
				}

				if (freerdp_client_start(context) != 0)
					goto out;

				thread = freerdp_client_get_thread(context);

				if (thread)
				{
					if (WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0)
					{


						dwCode = freerdp_get_last_error(context);

						if (dwCode == 131081)
						{
							wsprintf(szTemp, L"ip:%ls username:%-10.10ls password:%-6.6ls  fail\n", szip, szUserName, szPassWord);
							wprintf(L"%ls", szTemp);
						}
						else if (dwCode == 131075)
						{
							wsprintf(szTemp, L"ip:%ls username:%ls password:%ls  success\n", szip, szUserName, szPassWord);
							wprintf(L"%ls", szTemp);
							dwFlag = 1;
						}

						//MessageBoxW(NULL, szTemp, L"test", NULL);

						GetExitCodeThread(thread, &dwExitCode);
						ret = dwExitCode;
					}
				}

				if (freerdp_client_stop(context) != 0)
					goto out;

			}
			fclose(fpPassWord);

		}
		fclose(fpUserName);

	}
	fclose(fpIp);










	system("pause");
out:
	freerdp_client_context_free(context);

	if (argv)
	{
		for (i = 0; i < argc; i++)
			free(argv[i]);

		free(argv);
	}

	LocalFree(args);
	return ret;
}
