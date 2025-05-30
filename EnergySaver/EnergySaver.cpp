
// EnergySaver.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "EnergySaver.h"
#include "EnergySaverDlg.h"
#include "../include/EnergyManager.h"
#include "../include/function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEnergySaverApp

BEGIN_MESSAGE_MAP(CEnergySaverApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEnergySaverApp 构造

CEnergySaverApp::CEnergySaverApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CEnergySaverApp 对象

CEnergySaverApp theApp;


// CEnergySaverApp 初始化

BOOL CEnergySaverApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	Log::LogTitle("EnergySaver");

	//首先处理参数，如果有-Exit则关闭已经运行的程序
	bool exit=false;
	{
		int nArgs = 0;
		LPWSTR* szArglist = NULL;
		szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
		if (NULL != szArglist)
		{
			for (int i = 1; i < nArgs; ++i)
			{
				if (0 == _wcsicmp(L"-Exit", szArglist[i]))
				{
					exit=true;
				}
				if (0 == _wcsicmp(L"-fork", szArglist[i]))
				{
					//MyFunction::myCreateProcess(L"EnergySaver.exe");
					//MessageBox(NULL,TEXT("安装完毕后请重启电脑"), TEXT("提示"),MB_OK|MB_SETFOREGROUND);
					return FALSE;
				}
			}
		}
	}

	HANDLE hOneInstance;
	hOneInstance = ::CreateMutex(NULL, FALSE, TEXT("EnergySaver"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//AfxMessageBox(_T("应用程序已存在"));
		Log::WriteToLog("application runing already");

		//查找这个窗口,注意，这个变量窗口运行起来的标题！
		//修改对话框标题之后也要修改这个才会激活前一个实例
		HWND hWnd = FindWindow(NULL, TEXT("EnergySaver 节能减排"));
		if (IsWindow(hWnd))
		{
			//遍历处理所有同名窗口！！！
			while(IsWindow(hWnd))
			{
				stringstream ss;
				DWORD ProcessID;
				DWORD ThreadID;
				ThreadID=GetWindowThreadProcessId(hWnd,&ProcessID);
				ss.str()="";
				ss<<"window found ProcessID "<<ProcessID<<" ThreadID "<<ThreadID;
				Log::WriteToLog(ss.str().c_str());
				
				// 显示
				if(!::ShowWindow(hWnd, SW_NORMAL))
				{
					Log::WriteToLogW(L"ShowWindow error : ",GetLastError());
				} 
				// 激活
				if(!::SetForegroundWindow(hWnd))
				{
					Log::WriteToLogW(L"SetForegroundWindow error : ",GetLastError());
				}
				if(exit)
				{
					SendMessage(hWnd,WM_MY_EXIT,0,0);
					//HANDLE hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,ProcessID);
					//if(TerminateProcess(hProcess,0))
					//{
					//	Log::WriteToLog("TerminateProcess ok");
					//}
					//else
					//{
					//	Log::WriteToLogW(L"TerminateProcess error",GetLastError());
					//}
				}

				hWnd = FindWindowEx(NULL,hWnd,NULL,TEXT("EnergySaver 节能减排"));
			}
		}
		else
		{
			Log::WriteToLog("window not found");
		}

		return FALSE;
	}

	if(exit)
	{
		return FALSE;
	}

	CEnergySaverDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

