#pragma once
#include <windows.h>
#include <shlobj.h>

#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib") 

#include <string>
using std::wstring;
using std::string;

#include "function.h"

#define SERVIVE_NAME TEXT("EnergySaver") //服务名称
#define SLEEP_TIME 5000 //间隔时间，毫秒
#define SERVICE_TIME_TO_SUSPEND 300 //服务触发休眠时间，秒
#define SERVICE_TIME_PAUSE 300 //服务每次暂停时间，超时自动恢复
#define WM_MY_EXIT WM_APP+1 //关闭程序,不建议用WM_USER，开头几个会被系统无法预知地占用

//日志输出，不需要全局日志文件类，每次单独打开文件，不适用于连续输出大量日志的场合，输出为单字节字符集
class Log
{
public:
	//设置或判断是否是调试（与编译时的调试版本无关）
	static bool isDebug(bool* bSetDebug = NULL)
	{
		static bool _b = false;
		if (NULL != bSetDebug)
		{
			_b = *bSetDebug;
		}
		return _b;
	}
	//设置或获取日志标题，用于区分不同的程序
	static string LogTitle(char const* title = NULL)
	{
		static string _title;
		if (NULL != title)
		{
			_title = title;
		}
		return _title;
	}
	//获取数据目录位置，公共文档目录下面公司名下的服务名
	static wstring GetDataDir()
	{
		TCHAR szPath[MAX_PATH];
		int dir = CSIDL_COMMON_DOCUMENTS;//公共文档目录，能在服务和用户之间共享
		if (S_OK != SHGetFolderPath(NULL,
			dir | CSIDL_FLAG_CREATE,
			NULL,
			SHGFP_TYPE_CURRENT,
			szPath))
		{
			return TEXT("");
		}
		//wcscpy_s(szPath,TEXT("c:\\"));
		PathAppend(szPath, TEXT("CodeToys\\"));
		if (!PathIsDirectory(szPath))
		{
			if (!CreateDirectory(szPath, NULL))
			{
				MessageBox(NULL, szPath, TEXT("创建目录失败"), MB_OK);
			}
		}
		PathAppend(szPath, SERVIVE_NAME TEXT("\\"));
		if (!PathIsDirectory(szPath))
		{
			if (!CreateDirectory(szPath, NULL))
			{
				MessageBox(NULL, szPath, TEXT("创建目录失败"), MB_OK);
			}
		}
		//MessageBox(NULL,szPath,L"",MB_OK);
		return szPath;
	}
	//输出日志
	static int Log::WriteToLog(char const* str)
	{
		//return 0;

		wstring wfilename = GetDataDir();
		wfilename += L"\\log.txt";
		string filename;
		MyFunction::WStringToString(wfilename, filename);
		FILE* pfile;
		fopen_s(&pfile, filename.c_str(), "a+");
		if (pfile == NULL)
		{
			return -1;
		}
		time_t t1 = time(NULL);
		tm t2;
		char buf[2048];
		localtime_s(&t2, &t1);
		sprintf_s(buf, "%02d-%02d %02d:%02d:%02d", t2.tm_mon + 1, t2.tm_mday, t2.tm_hour, t2.tm_min, t2.tm_sec);

		fprintf_s(pfile, "[%6ld] %-20s [%s] %s\n", (long)GetCurrentProcessId(), LogTitle().c_str(), buf, str);
		fclose(pfile);

		return 0;
	}
	//输出日志，字符串和整数连续输出
	static int WriteToLogW(wchar_t const* wstr, long n)
	{
		wchar_t buf[256];
		wsprintf(buf, L"%ld", n);
		wstring tmp = wstr;
		tmp += buf;
		return WriteToLogW(tmp.c_str());
	}
	//字符串
	static int WriteToLogW(wchar_t const* wstr)
	{
		string str;
		MyFunction::WStringToString(wstr, str);
		return Log::WriteToLog(str.c_str());
	}
};
