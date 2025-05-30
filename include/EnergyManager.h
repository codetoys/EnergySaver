#pragma once
#include <windows.h>
#include <powrprof.h> //PowrProf.lib

class CEnergyManager
{
public:
	//控制是否是关机，否则是休眠
	static bool isShutdown() { return false; }
	static ULONGLONG GetLastInputPassed()
	{
		LASTINPUTINFO lastinfo;
		lastinfo.cbSize = sizeof(LASTINPUTINFO);
		if(!GetLastInputInfo(&lastinfo))
		{
			//
		}
		return (GetTickCount() - lastinfo.dwTime) / 1000;
	}
	//关闭计算机 
	static BOOL MySystemShutdown()
	{
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		//Get a token for this process. 
		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return(FALSE);
		//Get the LUID for the shutdown privilege. 
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
			&tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;  //one privilege to set    
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		//Get the shutdown privilege for this process. 
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
			(PTOKEN_PRIVILEGES)NULL, 0);
		if (GetLastError() != ERROR_SUCCESS)
			return FALSE;
		
		if (isShutdown())
		{
			//Shut down the system and force all applications to close. 
			if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
				SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
				SHTDN_REASON_MINOR_UPGRADE |
				SHTDN_REASON_FLAG_PLANNED))
				return FALSE;
		}
		else
		{
			//休眠（第一个参数false则睡眠）
			SetSuspendState(true, false, false);
		}
		return TRUE;
	}
};

