#pragma once
#include <windows.h>

#include <string>
using std::wstring;
using std::string;

class MyFunction
{
public:
	//执行外部程序不返回结果
	//static bool myCreateProcess(wchar_t const * cmd,wstring & err)
	//{
	//	err = L"";
	//	STARTUPINFO si;
	//	memset(&si,0,sizeof(STARTUPINFO));//初始化si在内存块中的值（详见memset函数）
	//	si.cb=sizeof(STARTUPINFO);
	//	si.dwFlags=STARTF_USESHOWWINDOW;
	//	si.wShowWindow=SW_NORMAL;
	//	PROCESS_INFORMATION pi;//必备参数设置结束
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

	//执行外部程序并返回结果
	static int mySystem(wchar_t const * cmd,wstring & err)
	{
		err = L"";
		STARTUPINFO si;
		memset(&si,0,sizeof(STARTUPINFO));//初始化si在内存块中的值（详见memset函数）
		si.cb=sizeof(STARTUPINFO);
		si.dwFlags=STARTF_USESHOWWINDOW;
		si.wShowWindow=SW_HIDE;
		PROCESS_INFORMATION pi;//必备参数设置结束
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

	//将整数转换到宽字符串
	static wstring ToWString(long value)
	{
		wchar_t buf[256];
		wsprintf(buf, L"%ld", value);
		return buf;
	}
	//宽字节字符串转单字节字符串
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
