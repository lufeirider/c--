VmpKillProcess(HANDLE hProc)
{
    DWORD MaxAddr;
    DWORD CurAddr;
    DWORD PageSize;
    DWORD OldProtect;    
	SYSTEM_INFO SysInfo;
    MEMORY_BASIC_INFORMATION MemBasicInfo;
    
	if (hProc <= 0 ) 
	{
		printf("hProc error ");
		return FALSE;
	}
	GetSystemInfo(&SysInfo);
    MaxAddr = (DWORD)SysInfo.lpMaximumApplicationAddress;
    PageSize = SysInfo.dwPageSize;    
	for (CurAddr = (DWORD)SysInfo.lpMinimumApplicationAddress; CurAddr <= MaxAddr; CurAddr += PageSize)
    {        //
        // Main loop: if the address is valid, make it unaccessible
        if (VirtualQueryEx(hProc, (LPVOID)(CurAddr), &MemBasicInfo, sizeof(MEMORY_BASIC_INFORMATION)))
        {
			//MemBasicInfo.BaseAddress = (void *)0x003f0000;
			printf("%x\n",MemBasicInfo.BaseAddress);
			//getchar();

            VirtualProtectEx(hProc, MemBasicInfo.BaseAddress, MemBasicInfo.RegionSize, PAGE_NOACCESS, &OldProtect);
            CurAddr += MemBasicInfo.RegionSize;
        }
    }    return TRUE;
}
