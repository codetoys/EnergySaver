// EnergySaverService.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <time.h>
using std::wstring;
using std::string;
#include "../include/log.h"
#include "../include/EnergyManager.h"
#include "../include/ServiceFunction.h"

bool bRun = false;//服务进程运行，false则退出服务程序，服务暂停并不会暂停服务进程，服务进程自行决定暂停行为
bool bPause = false;//暂停状态由服务控制设置，但是超过SERVICE_TIME_PAUSE就自动恢复
time_t pause_time = 0;//服务暂停了多久

SERVICE_STATUS serviceStatus;

SERVICE_STATUS_HANDLE hStatus;

void WINAPI CtrlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		bRun = false;
		Log::WriteToLog("SERVICE_CONTROL_STOP");
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		bRun = false;
		Log::WriteToLog("SERVICE_CONTROL_SHUTDOWN");
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		break;
	case SERVICE_CONTROL_PAUSE:
		bRun = true;
		pause_time = time(NULL);
		bPause = true;
		Log::WriteToLog("SERVICE_CONTROL_PAUSE");
		serviceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		bRun = true;
		bPause = false;
		pause_time = 0;
		Log::WriteToLog("SERVICE_CONTROL_CONTINUE");
		serviceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	default:
		break;
	}

	SetServiceStatus(hStatus, &serviceStatus);
}

void WINAPI ServiceMain(int argc, char** argv)
{
	Log::LogTitle("EnergySaverService");

	serviceStatus.dwServiceType = SERVICE_WIN32;
	serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	serviceStatus.dwWin32ExitCode = 0;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	hStatus = ::RegisterServiceCtrlHandler(SERVIVE_NAME, CtrlHandler);
	if (hStatus == 0)
	{
		Log::WriteToLog("RegisterServiceCtrlHandler failed");
		return;
	}

	Log::WriteToLog("RegisterServiceCtrlHandler success");

	//向SCM 报告运行状态
	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &serviceStatus);

	//服务做的工作
	bRun = true;
	int count = 0;

	for (; bRun; Sleep(SLEEP_TIME))
	{
		time_t t1 = time(NULL);

		if (bPause)
		{
			if (t1 - pause_time >= SERVICE_TIME_PAUSE)
			{
				bPause = false;
				pause_time = 0;
				count = 0;
				//向SCM 报告运行状态
				serviceStatus.dwCurrentState = SERVICE_RUNNING;
				SetServiceStatus(hStatus, &serviceStatus);
				Log::WriteToLog("service atuo continue");
			}
			else
			{
				count = 0;
			}
		}
		else
		{
			ServiceFunction sf;
			sf.init();//读配置文件获取应用提供的最后输入时间
			if (time(NULL) - sf.last_input < 180)
			{//桌面应用存在，自动暂停
				pause_time = time(NULL);
				bPause = true;
				serviceStatus.dwCurrentState = SERVICE_PAUSED;
				SetServiceStatus(hStatus, &serviceStatus);
				Log::WriteToLogW(L"service atuo pause ", time(NULL) - sf.last_input);
			}
			else
			{
				Log::WriteToLogW(L"user app not exist ", time(NULL) - sf.last_input);
				count += SLEEP_TIME;
				if (count >= SERVICE_TIME_TO_SUSPEND * 1000)
				{
					count = 0;
					Log::WriteToLog("SUSPEND by service");
					CEnergyManager::MySystemShutdown();
				}
			}
		}
	}

	Log::WriteToLog("service stopped");
}

int _tmain(int argc, _TCHAR* argv[])
{
	SERVICE_TABLE_ENTRY entrytable[2];

	entrytable[0].lpServiceName = SERVIVE_NAME;
	entrytable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
	entrytable[1].lpServiceName = NULL;
	entrytable[1].lpServiceProc = NULL;

	StartServiceCtrlDispatcher(entrytable);

	return 0;
}
