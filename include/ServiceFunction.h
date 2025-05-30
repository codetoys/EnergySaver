#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <time.h>
#include <sstream>
using std::wstring;
using std::string;
using std::wstringstream;
using std::stringstream;
#include "function.h"
#include "config.h"
#include "EnergyManager.h"

class ServiceFunction
{
private:
public:
	int IDEL_TIME_TO_SUSPEND;//无操作时间，秒
	bool IDEL_TIME_LOCK;//无操作时锁屏
	bool IP_REPORT;//报告IP地址
	int NO_SUSPEND_FROM;
	int NO_SUSPEND_TO;
	time_t OVERTIME_TO;//加班截止时间

	time_t last_input;//上次操作时间，此数值有所有已登录用户写入，服务根据这个判断是否超时，由于服务是随系统启动的特殊程序，普通交互方式不可用

	bool init()
	{
		wstring str;
		if (!CConfig::GetConfig(L"IDEL_TIME_TO_SUSPEND", L"300", str))
		{
			return false;
		}
		IDEL_TIME_TO_SUSPEND = _wtol(str.c_str());

		if (!CConfig::GetConfig(L"IDEL_TIME_LOCK", L"TRUE", str))
		{
			return false;
		}
		IDEL_TIME_LOCK = (L"TRUE" == str);
	
		if (!CConfig::GetConfig(L"IP_REPORT", L"TRUE", str))
		{
			return false;
		}
		IP_REPORT = (L"TRUE" == str);

		if (!CConfig::GetConfig(L"NO_SUSPEND_FROM", L"8", str))
		{
			return false;
		}
		NO_SUSPEND_FROM = _wtol(str.c_str());

		if (!CConfig::GetConfig(L"NO_SUSPEND_TO", L"18", str))
		{
			return false;
		}
		NO_SUSPEND_TO = _wtol(str.c_str());

		if (!CConfig::GetConfig(L"OVERTIME_TO", L"", str))
		{
			return false;
		}
		OVERTIME_TO = _wtol(str.c_str());

		if (!CConfig::GetConfig(L"last_input", L"", str))
		{
			return false;
		}
		last_input = _wtol(str.c_str());

		if (!CConfig::GetConfig(L"DEBUG", L"false", str))
		{
			return false;
		}
		bool b = (L"true" == str);
		Log::isDebug(&b);

		return true;
	}
	bool setLastInputToFile(time_t t1)
	{
		return CConfig::SetConfig(L"last_input", MyFunction::ToWString((long)t1).c_str());
	}
	bool isWorkTime(time_t const& t1)
	{
		tm t2;
		localtime_s(&t2, &t1);

		if (NO_SUSPEND_FROM == NO_SUSPEND_TO)
		{
			return false;
		}
		else if (NO_SUSPEND_FROM < NO_SUSPEND_TO)
		{
			return (t2.tm_hour >= NO_SUSPEND_FROM && t2.tm_hour < NO_SUSPEND_TO);
		}
		else
		{
			return (t2.tm_hour >= NO_SUSPEND_FROM || t2.tm_hour < NO_SUSPEND_TO);
		}
	}
	//返回值表示是否执行了休眠
	bool doServiceFunction(ULONGLONG n, bool& soon_to_suspend, int& time_to_suspend, int& after_suspend)
	{
		bool ret = false;
		soon_to_suspend = false;

		static int m_time_to_suspend = 0;//准备休眠的时间
		static int m_after_suspend = 0;//休眠恢复后不休眠的时间，以免在恢复后不留神又休眠

		time_t t1 = time(NULL);

		stringstream ss;
		ss << IDEL_TIME_TO_SUSPEND << " OVERTIME_TO=" << OVERTIME_TO << " m_time_to_suspend=" << m_time_to_suspend << " m_after_suspend=" << m_after_suspend
			<< " n=" << n << " isWorkTime=" << (isWorkTime(t1) ? "true" : "false") << " (" << NO_SUSPEND_FROM << "-" << NO_SUSPEND_TO << ")";
		if (Log::isDebug())Log::WriteToLog(ss.str().c_str());

		if (n >= IDEL_TIME_TO_SUSPEND && (0 == OVERTIME_TO || t1 > OVERTIME_TO) && 0 >= m_after_suspend && !isWorkTime(t1))
		{
			if (m_time_to_suspend > 0)
			{
				soon_to_suspend = true;
				m_time_to_suspend -= (0 == SLEEP_TIME / 1000 ? 1 : SLEEP_TIME / 1000);
				if (m_time_to_suspend <= 0)
				{
					soon_to_suspend = false;
					Log::WriteToLog("Suspend");
					m_time_to_suspend = 0;
					m_after_suspend = 300;
					CEnergyManager em;
					em.MySystemShutdown();
					ret = true;
				}
			}
			else
			{
				m_time_to_suspend = 60;
			}
		}
		else
		{
			m_time_to_suspend = 0;
			if (m_after_suspend > 0)m_after_suspend -= (0 == SLEEP_TIME / 1000 ? 1 : SLEEP_TIME / 1000);
		}

		time_to_suspend = m_time_to_suspend;
		after_suspend = m_after_suspend;
		return ret;
	}
};

