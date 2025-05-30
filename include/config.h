#pragma once
#include <windows.h>
#include "log.h"
#include <string>
using std::wstring;

class CConfig
{
public:
	static bool GetConfig(LPCWSTR name,LPCWSTR default_value,wstring & value)
	{
		wstring wfilename=Log::GetDataDir();
		wfilename+=L"\\config.ini";
		wchar_t buf[2048];
		DWORD n=GetPrivateProfileStringW(L"EnergyManager",name,default_value,buf,2048,wfilename.c_str());
		if(n<0)
		{
			Log::WriteToLogW(wfilename.c_str());
			return false;
		}
		buf[n]=L'\0';
		value=buf;
		return true;
	}
	static bool SetConfig(LPCWSTR name,LPCWSTR value)
	{
		wstring wfilename=Log::GetDataDir();
		wfilename+=L"\\config.ini";
		if(!WritePrivateProfileStringW(L"EnergyManager",name,value,wfilename.c_str()))
		{
			Log::WriteToLogW(wfilename.c_str());
			return false;
		}
		return true;
	}
};
