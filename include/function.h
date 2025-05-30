#pragma once
#include <windows.h>

#include <string>
using std::wstring;
using std::string;

class MyFunction
{
public:
	//ִ���ⲿ���򲻷��ؽ��
	//static bool myCreateProcess(wchar_t const * cmd,wstring & err)
	//{
	//	err = L"";
	//	STARTUPINFO si;
	//	memset(&si,0,sizeof(STARTUPINFO));//��ʼ��si���ڴ���е�ֵ�����memset������
	//	si.cb=sizeof(STARTUPINFO);
	//	si.dwFlags=STARTF_USESHOWWINDOW;
	//	si.wShowWindow=SW_NORMAL;
	//	PROCESS_INFORMATION pi;//�ر��������ý���
	//	//Log::WriteToLogW(cmd);
	//	wchar_t buf[2048];
	//	wcscpy_s(buf,cmd);
	//	if(!CreateProcess(NULL,buf,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
	//	{
	//		err+=ToWString(GetLastError());
	//		CloseHandle(pi.hThread);
	//		CloseHandle(pi.hProcess);
	//		return false;
	//	}
	//	CloseHandle(pi.hThread);
	//	CloseHandle(pi.hProcess);
	//	return true;
	//}

	//ִ���ⲿ���򲢷��ؽ��
	static int mySystem(wchar_t const * cmd,wstring & err)
	{
		err = L"";
		STARTUPINFO si;
		memset(&si,0,sizeof(STARTUPINFO));//��ʼ��si���ڴ���е�ֵ�����memset������
		si.cb=sizeof(STARTUPINFO);
		si.dwFlags=STARTF_USESHOWWINDOW;
		si.wShowWindow=SW_HIDE;
		PROCESS_INFORMATION pi;//�ر��������ý���
		//Log::WriteToLogW(cmd);
		wchar_t buf[2048];
		wcscpy_s(buf,cmd);
		if(!CreateProcess(NULL,buf,NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi))
		{
			err+=ToWString(GetLastError());
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			return -1;
		}
		DWORD dwRet;
		while(true)
		{
			//Log::WriteToLogW(L"GetExitCodeProcess...");
			if(!GetExitCodeProcess(pi.hProcess,&dwRet))
			{
				err+=L"GetExitCodeProcess error";
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
				return -2;
			}
			if(STILL_ACTIVE==dwRet)continue;
			else break;
		}
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return dwRet;
	}

	//������ת�������ַ���
	static wstring ToWString(long value)
	{
		wchar_t buf[256];
		wsprintf(buf, L"%ld", value);
		return buf;
	}
	//���ֽ��ַ���ת���ֽ��ַ���
	static string WStringToString(const std::wstring& wstr, std::string& str)
	{
		LPCWSTR pwszSrc = wstr.c_str();
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
		if (nLen == 0)
			return std::string("");

		char* pszDst = new char[nLen];
		if (!pszDst)
			return std::string("");

		WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
		str = pszDst;
		delete[] pszDst;
		pszDst = NULL;

		return str;
	}
};
