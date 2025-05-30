#pragma once
#include <windows.h>
#include <shlobj.h>

#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib") 

#include <string>
using std::wstring;
using std::string;

#include "function.h"

#define SERVIVE_NAME TEXT("EnergySaver") //��������
#define SLEEP_TIME 5000 //���ʱ�䣬����
#define SERVICE_TIME_TO_SUSPEND 300 //���񴥷�����ʱ�䣬��
#define SERVICE_TIME_PAUSE 300 //����ÿ����ͣʱ�䣬��ʱ�Զ��ָ�
#define WM_MY_EXIT WM_APP+1 //�رճ���,��������WM_USER����ͷ�����ᱻϵͳ�޷�Ԥ֪��ռ��

//��־���������Ҫȫ����־�ļ��࣬ÿ�ε������ļ������������������������־�ĳ��ϣ����Ϊ���ֽ��ַ���
class Log
{
public:
	//���û��ж��Ƿ��ǵ��ԣ������ʱ�ĵ��԰汾�޹أ�
	static bool isDebug(bool* bSetDebug = NULL)
	{
		static bool _b = false;
		if (NULL != bSetDebug)
		{
			_b = *bSetDebug;
		}
		return _b;
	}
	//���û��ȡ��־���⣬�������ֲ�ͬ�ĳ���
	static string LogTitle(char const* title = NULL)
	{
		static string _title;
		if (NULL != title)
		{
			_title = title;
		}
		return _title;
	}
	//��ȡ����Ŀ¼λ�ã������ĵ�Ŀ¼���湫˾���µķ�����
	static wstring GetDataDir()
	{
		TCHAR szPath[MAX_PATH];
		int dir = CSIDL_COMMON_DOCUMENTS;//�����ĵ�Ŀ¼�����ڷ�����û�֮�乲��
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
				MessageBox(NULL, szPath, TEXT("����Ŀ¼ʧ��"), MB_OK);
			}
		}
		PathAppend(szPath, SERVIVE_NAME TEXT("\\"));
		if (!PathIsDirectory(szPath))
		{
			if (!CreateDirectory(szPath, NULL))
			{
				MessageBox(NULL, szPath, TEXT("����Ŀ¼ʧ��"), MB_OK);
			}
		}
		//MessageBox(NULL,szPath,L"",MB_OK);
		return szPath;
	}
	//�����־
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
	//�����־���ַ����������������
	static int WriteToLogW(wchar_t const* wstr, long n)
	{
		wchar_t buf[256];
		wsprintf(buf, L"%ld", n);
		wstring tmp = wstr;
		tmp += buf;
		return WriteToLogW(tmp.c_str());
	}
	//�ַ���
	static int WriteToLogW(wchar_t const* wstr)
	{
		string str;
		MyFunction::WStringToString(wstr, str);
		return Log::WriteToLog(str.c_str());
	}
};
