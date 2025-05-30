// EnergySaverCtrl.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <time.h>
#include "../include/log.h"
#include "../include/function.h"
using std::wstring;

int install(int argc, _TCHAR* argv[])
{
	wstring wstr;
	wchar_t buf[2048];
	int n;

	//构造服务程序的路径名
	wstring servicefilename = argv[0];
	servicefilename.erase(servicefilename.find_last_of(L'\\'));
	servicefilename += L"\\EnergySaverService.exe";

	wstr = L"sc.exe Create \"";
	wstr += SERVIVE_NAME;
	wstr += L"\" binpath= \"";
	wstr += servicefilename;
	wstr += L"\" start= auto type= own type= interact";

	wstring err;
	n = MyFunction::mySystem(wstr.c_str(), err);
	wsprintf(buf, L"%s : %d : %s", wstr.c_str(), n, err.c_str());
	Log::WriteToLogW(buf);
	if (n != 0)
	{
		Log::WriteToLog("Install error");
		return 1;
	}
	else
	{
		Log::WriteToLog("Install ok");
	}
	return 0;
}
int uninstall()
{
	wstring wstr;
	wchar_t buf[2048];
	int n;

	wstr = L"sc.exe delete \"";
	wstr += SERVIVE_NAME;
	wstr += L"\"";

	wstring err;
	n = MyFunction::mySystem(wstr.c_str(), err);
	wsprintf(buf, L"%s : %d : %d", wstr.c_str(), n, err.c_str());
	Log::WriteToLogW(buf);
	if (n != 0)
	{
		Log::WriteToLog("Uninstall error");
		return 1;
	}
	else
	{
		Log::WriteToLog("Uninstall ok");
		return 0;
	}
}
int start()
{
	wstring wstr;
	wchar_t buf[2048];
	int n;

	wstr = L"sc.exe start \"";
	wstr += SERVIVE_NAME;
	wstr += L"\"";

	wstring err;
	n = MyFunction::mySystem(wstr.c_str(), err);
	wsprintf(buf, L"%s : %d : %s", wstr.c_str(), n, err.c_str());
	Log::WriteToLogW(buf);
	if (n != 0)
	{
		Log::WriteToLog("Service start error");
		return 2;
	}
	else
	{
		Log::WriteToLog("Service start ok");
		return 0;
	}
}
int stop()
{
	wstring wstr;
	wchar_t buf[2048];
	int n;

	wstr = L"sc.exe stop \"";
	wstr += SERVIVE_NAME;
	wstr += L"\"";

	wstring err;
	n = MyFunction::mySystem(wstr.c_str(), err);
	wsprintf(buf, L"%s : %d : %s", wstr.c_str(), n, err.c_str());
	Log::WriteToLogW(buf);
	if (n != 0)
	{
		Log::WriteToLog("Service stop error");
		return 2;
	}
	else
	{
		Log::WriteToLog("Service stop ok");
	}
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	Log::LogTitle("EnergySaverCtrl");

	for (int i = 1; i < argc; ++i)
	{
		wstring wstr = argv[i];
		string str;
		MyFunction::WStringToString(wstr, str);
		if ("/Install" == str)
		{
			if (0 != install(argc, argv))
			{
				return 1;
			}
			//if(0!=start())
			//{
			//	return 2;
			//}
			return 0;
		}
		else if (("/Uninstall" == str))
		{
			stop();
			uninstall();
			return 0;
		}
		else if ("/Restart" == str)
		{
			stop();
			return start();
		}

	}

	Log::WriteToLog("Unknown param");

	return 1;
}

/*
SC命令：
sc Create "name" binpath= "path"  displayname= "SVNService" depend= Tcpip start=auto

sc delete 服务名称

C:\Windows\System32\sc.exe Create "aaa" binpath="F:\tmp\aa\Debug\aa.exe" displayname="aaa"

输入sc create testservicename binpath= D:\test.exe (这里的D:\test.exe路径就是你的.exe程序所在的绝对路径，注意"="后面有一个空格)

输入sc start testservicename 启动服务

输入sc query 会在最底部显示你的服务当前的状态

输入sc stop testservicename 停止服务

输入sc delete testservicename删除服务，该服务将在下次重启后删除，在重启之前将不能注册同一个名字的服务。

*/
