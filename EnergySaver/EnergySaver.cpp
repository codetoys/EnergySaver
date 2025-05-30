
// EnergySaver.cpp : ����Ӧ�ó��������Ϊ��
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


// CEnergySaverApp ����

CEnergySaverApp::CEnergySaverApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CEnergySaverApp ����

CEnergySaverApp theApp;


// CEnergySaverApp ��ʼ��

BOOL CEnergySaverApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	Log::LogTitle("EnergySaver");

	//���ȴ�������������-Exit��ر��Ѿ����еĳ���
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
					//MessageBox(NULL,TEXT("��װ��Ϻ�����������"), TEXT("��ʾ"),MB_OK|MB_SETFOREGROUND);
					return FALSE;
				}
			}
		}
	}

	HANDLE hOneInstance;
	hOneInstance = ::CreateMutex(NULL, FALSE, TEXT("EnergySaver"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//AfxMessageBox(_T("Ӧ�ó����Ѵ���"));
		Log::WriteToLog("application runing already");

		//�����������,ע�⣬��������������������ı��⣡
		//�޸ĶԻ������֮��ҲҪ�޸�����Żἤ��ǰһ��ʵ��
		HWND hWnd = FindWindow(NULL, TEXT("EnergySaver ���ܼ���"));
		if (IsWindow(hWnd))
		{
			//������������ͬ�����ڣ�����
			while(IsWindow(hWnd))
			{
				stringstream ss;
				DWORD ProcessID;
				DWORD ThreadID;
				ThreadID=GetWindowThreadProcessId(hWnd,&ProcessID);
				ss.str()="";
				ss<<"window found ProcessID "<<ProcessID<<" ThreadID "<<ThreadID;
				Log::WriteToLog(ss.str().c_str());
				
				// ��ʾ
				if(!::ShowWindow(hWnd, SW_NORMAL))
				{
					Log::WriteToLogW(L"ShowWindow error : ",GetLastError());
				} 
				// ����
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

				hWnd = FindWindowEx(NULL,hWnd,NULL,TEXT("EnergySaver ���ܼ���"));
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
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

