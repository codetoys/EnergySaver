// EnergySaverCtrl.cpp : �������̨Ӧ�ó������ڵ㡣
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

	//�����������·����
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
SC���
sc Create "name" binpath= "path"  displayname= "SVNService" depend= Tcpip start=auto

sc delete ��������

C:\Windows\System32\sc.exe Create "aaa" binpath="F:\tmp\aa\Debug\aa.exe" displayname="aaa"

����sc create testservicename binpath= D:\test.exe (�����D:\test.exe·���������.exe�������ڵľ���·����ע��"="������һ���ո�)

����sc start testservicename ��������

����sc query ������ײ���ʾ��ķ���ǰ��״̬

����sc stop testservicename ֹͣ����

����sc delete testservicenameɾ�����񣬸÷������´�������ɾ����������֮ǰ������ע��ͬһ�����ֵķ���

*/
