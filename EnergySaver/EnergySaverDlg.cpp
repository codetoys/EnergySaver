
// EnergySaverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EnergySaver.h"
#include "EnergySaverDlg.h"
#include "afxdialogex.h"
#include "../include/EnergyManager.h"
#include "../include/function.h"
#include <winsvc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEnergySaverDlg �Ի���




CEnergySaverDlg::CEnergySaverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnergySaverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Initing = true;
}

void CEnergySaverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LAST, m_edit_last);
	DDX_Control(pDX, IDC_EDIT_SERVICE_STATE, m_Edit_ServiceState);
	DDX_Control(pDX, IDC_EDIT_IDEL_TIME_TO_SUSPEND, m_Edit_IdelTimeToSuspend);
	DDX_Control(pDX, IDC_COMBO_NO_SUSPEND_FROM, m_ComboBox_NoSuspendFrom);
	DDX_Control(pDX, IDC_COMBO_NO_SUSPEND_TO, m_ComboBox_NoSuspendTo);
	DDX_Control(pDX, IDC_EDIT_WORK_OVERTIME, m_Edit_WorkOvertime);
	DDX_Control(pDX, IDC_EDIT_OVERTIME_STATE, m_Edit_OvertimeState);
	DDX_Control(pDX, IDC_STATIC_OP, m_static_op);
	DDX_Control(pDX, IDC_CHECK_LOCK, m_Check_Lock);
	DDX_Control(pDX, IDC_SYSLINK_WEBSITE, m_SysLink_Website);
}

BEGIN_MESSAGE_MAP(CEnergySaverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_WORK_OVERTIME, &CEnergySaverDlg::OnBnClickedButtonWorkOvertime)
	ON_BN_CLICKED(IDC_BUTTON_WORK_OVERTIME_CANCEL, &CEnergySaverDlg::OnBnClickedButtonWorkOvertimeCancel)
	ON_BN_CLICKED(IDOK, &CEnergySaverDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEnergySaverDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_IDEL_TIME_TO_SUSPEND, &CEnergySaverDlg::OnEnChangeEditIdelTimeToSuspend)
	ON_CBN_SELCHANGE(IDC_COMBO_NO_SUSPEND_FROM, &CEnergySaverDlg::OnCbnSelchangeComboNoSuspendFrom)
	ON_CBN_SELCHANGE(IDC_COMBO_NO_SUSPEND_TO, &CEnergySaverDlg::OnCbnSelchangeComboNoSuspendTo)
	ON_MESSAGE(WM_MY_EXIT, OnMyExit)
	ON_STN_CLICKED(IDC_STATIC_ICON, &CEnergySaverDlg::OnStnClickedStaticIcon)
	ON_BN_CLICKED(ID_BUTTON_LOCK, &CEnergySaverDlg::OnBnClickedButtonLock)
	ON_BN_CLICKED(IDC_CHECK_LOCK, &CEnergySaverDlg::OnBnClickedCheckLock)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_WEBSITE, &CEnergySaverDlg::OnNMClickSyslinkWebsite)
END_MESSAGE_MAP()


// CEnergySaverDlg ��Ϣ�������

BOOL CEnergySaverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//ʹ���ڲ�������������
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	//���ȴ�������������-auto�����ش���
	{
		int nArgs = 0;
		LPWSTR* szArglist = NULL;
		szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
		isStartHide = false;
		if (NULL != szArglist)
		{
			for (int i = 1; i < nArgs; ++i)
			{
				if (0 == _wcsicmp(L"-Auto", szArglist[i]))
				{
					isStartHide = true;
					GetWindowRect(&rt);//���ڴ�ʱ������δ��ʾ����õ�λ���ڣ�0,0����������ʾʱ��Ҫ��CenterWindow�ƶ�������
					//CString str;
					//str.Format(L"%ld,%ld,%ld,%ld",rt.left,rt.top,rt.bottom,rt.right);
					//MessageBox(str);
					MoveWindow(0, 0, 0, 0);
				}
			}
		}
	}
	if (!sf.init())
	{
		this->MessageBox(TEXT("��ʼ��ʧ��"));
	}

	//Ԥ������������
	for (int i = 0; i <= 24; ++i)
	{
		CString cs;
		cs.Format(TEXT("%d"), i);
		this->m_ComboBox_NoSuspendFrom.AddString(cs);
		this->m_ComboBox_NoSuspendTo.AddString(cs);
	}

	this->m_Edit_WorkOvertime.SetWindowText(TEXT("4"));
	this->m_Edit_IdelTimeToSuspend.SetWindowText(MyFunction::ToWString(sf.IDEL_TIME_TO_SUSPEND).c_str());
	if (sf.IDEL_TIME_LOCK)this->m_Check_Lock.SetCheck(BST_CHECKED);
	else this->m_Check_Lock.SetCheck(BST_UNCHECKED);
	if (sf.IP_REPORT)this->m_Check_IP_Report.SetCheck(BST_CHECKED);
	else this->m_Check_IP_Report.SetCheck(BST_UNCHECKED);
	this->m_ComboBox_NoSuspendFrom.SelectString(-1, MyFunction::ToWString(sf.NO_SUSPEND_FROM).c_str());
	this->m_ComboBox_NoSuspendTo.SelectString(-1, MyFunction::ToWString(sf.NO_SUSPEND_TO).c_str());

	this->m_static_op.SetWindowText(CEnergyManager::isShutdown() ? TEXT("�ػ�") : TEXT("����"));

	SetTimer(1, SLEEP_TIME, 0);

	m_Initing = false;
	ShowOvertimeState();
	ShowServiceState();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEnergySaverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEnergySaverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEnergySaverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEnergySaverDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		if (isStartHide)
		{
			isStartHide = false;

			ShowWindow(SW_HIDE);//����
			MoveWindow(rt);//�ָ�λ��
			CenterWindow();//�ƶ�������
		}

		ShowOvertimeState();
		ShowServiceState();

		time_t t1 = time(NULL);
		CEnergyManager em;
		ULONGLONG n = em.GetLastInputPassed();
		sf.setLastInputToFile(t1);//��¼���ļ��Ա�����ܹ����,ʵ�ʼ�¼���ǵ�ǰʱ�䣬��������������

		bool soon_to_suspend;
		int time_to_suspend;
		int after_suspend;
		if (sf.doServiceFunction(n, soon_to_suspend, time_to_suspend, after_suspend))
		{
			ShowWindow(SW_HIDE);//˯�ߺ�Ҫ���ش��ڣ�����ָ�ϵͳ��������ʾ
		}
		else
		{
			if (n > sf.IDEL_TIME_TO_SUSPEND && BST_CHECKED == m_Check_Lock.GetCheck())
			{
				OnBnClickedButtonLock();//��������
			}
		}

		CString str;
		if (soon_to_suspend)
		{
			//�������������δ����������ϵͳ��������¼����
			::PostMessage(GetDesktopWindow()->GetSafeHwnd(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);
			//ShowWindow(SW_SHOW);
			//SetForegroundWindow();
		}
		if (time_to_suspend > 0 || after_suspend > 0)
		{
			str.Format(TEXT("%d ������� %d"), time_to_suspend, after_suspend);
		}
		else
		{
			str.Format(TEXT("�ϴζ����� %lld ��ǰ"), n);
		}
		m_edit_last.SetWindowText(str);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CEnergySaverDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox(L"OnBnClickedOk");
	if (IDYES == AfxMessageBox(TEXT("�˳����򽫲���ʵ�ֽ��ܼ��ţ�ȷ��Ҫ�˳���"), MB_YESNO | MB_ICONEXCLAMATION))
	{
		CDialogEx::OnOK();
	}
}


void CEnergySaverDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox(L"OnBnClickedCancel");
	ShowWindow(SW_HIDE);
	//CDialogEx::OnCancel();
}
LRESULT CEnergySaverDlg::OnMyExit(WPARAM w, LPARAM l)
{
	Log::WriteToLog("RECIVE WM_MY_EXIT");
	CDialogEx::OnOK();
	return 99;
}
void CEnergySaverDlg::OnBnClickedButtonWorkOvertime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cs;
	this->m_Edit_WorkOvertime.GetWindowText(cs);
	time_t t1 = time(NULL);
	if (t1 <= sf.OVERTIME_TO)
	{
		sf.OVERTIME_TO += 3600 * _ttol(cs);
	}
	else
	{
		sf.OVERTIME_TO = t1 + 3600 * _ttol(cs);
	}
	ShowOvertimeState();
	Save();
}


void CEnergySaverDlg::OnBnClickedButtonWorkOvertimeCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	sf.OVERTIME_TO = 0;
	ShowOvertimeState();
	Save();
}
void CEnergySaverDlg::ShowOvertimeState()
{
	time_t t1 = time(NULL);
	if (t1 >= sf.OVERTIME_TO)
	{
		this->m_Edit_OvertimeState.SetWindowText(TEXT("�ǼӰ���"));
	}
	else
	{
		CString str;
		time_t remain = sf.OVERTIME_TO - t1;
		str.Format(TEXT("�Ӱ��У�ʣ��%lldСʱ%lld����"), remain / 3600, (remain % 3600) / 60);
		this->m_Edit_OvertimeState.SetWindowText(str);
	}
}
DWORD CEnergySaverDlg::GetServiceState()
{
	//��ѯ����״̬
	SC_HANDLE hSCManager;
	SC_HANDLE hService;
	SERVICE_STATUS status;

	hSCManager = OpenSCManager(NULL, NULL, 0);
	if (NULL != hSCManager)
	{
		hService = OpenService(hSCManager, SERVIVE_NAME, SERVICE_QUERY_STATUS);
		if (NULL != hService)
		{
			if (!QueryServiceStatus(hService, &status))
			{
				this->m_Edit_ServiceState.SetWindowText(TEXT("QueryServiceStatusʧ��"));
			}
			else
			{
				return status.dwCurrentState;
			}
		}
		else
		{
			this->m_Edit_ServiceState.SetWindowText(TEXT("OpenServiceʧ��"));
		}
	}
	else
	{
		this->m_Edit_ServiceState.SetWindowText(TEXT("OpenSCManagerʧ��"));
	}

	m_ServiceInstalled = false;

	return 0;
}
void CEnergySaverDlg::ShowServiceState()
{

	switch (GetServiceState())
	{
	case SERVICE_CONTINUE_PENDING:
		this->m_Edit_ServiceState.SetWindowText(TEXT("SERVICE_CONTINUE_PENDING"));
		break;
	case SERVICE_PAUSE_PENDING:
		this->m_Edit_ServiceState.SetWindowText(TEXT("SERVICE_PAUSE_PENDING"));
		break;
	case SERVICE_PAUSED:
		this->m_Edit_ServiceState.SetWindowText(TEXT("����"));
		break;
	case SERVICE_RUNNING:
		this->m_Edit_ServiceState.SetWindowText(TEXT("SERVICE_RUNNING"));
		break;
	case SERVICE_START_PENDING:
		this->m_Edit_ServiceState.SetWindowText(TEXT("SERVICE_START_PENDING"));
		break;
	case SERVICE_STOP_PENDING:
		this->m_Edit_ServiceState.SetWindowText(TEXT("SERVICE_STOP_PENDING"));
		break;
	case SERVICE_STOPPED:
		this->m_Edit_ServiceState.SetWindowText(TEXT("SERVICE_STOPPED"));
		break;
	}

	if (m_ServiceInstalled)
	{
		this->m_SysLink_Website.SetWindowTextW(TEXT("<a href=\"https://codetoys.github.io/EnergySaver/\">�����Ѱ�װ</a>"));
	}
	else
	{
		this->m_SysLink_Website.SetWindowTextW(TEXT("<a href=\"https://codetoys.github.io/EnergySaver/\">����δ��װ����������վ</a>"));
	}
}

bool CEnergySaverDlg::CheckChanged()
{
	CString cs;

	sf.init();

	this->m_Edit_IdelTimeToSuspend.GetWindowText(cs);
	if (MyFunction::ToWString(sf.IDEL_TIME_TO_SUSPEND).c_str() != cs)return true;

	int check_state_lock = this->m_Check_Lock.GetCheck();
	if (BST_CHECKED == check_state_lock && !sf.IDEL_TIME_LOCK)return true;
	if (BST_CHECKED != check_state_lock && sf.IDEL_TIME_LOCK)return true;

	int check_state_ip_report = this->m_Check_IP_Report.GetCheck();
	if (BST_CHECKED == check_state_ip_report && !sf.IP_REPORT)return true;
	if (BST_CHECKED != check_state_ip_report && sf.IP_REPORT)return true;

	this->m_ComboBox_NoSuspendFrom.GetWindowText(cs);
	if (MyFunction::ToWString(sf.NO_SUSPEND_FROM).c_str() != cs)return true;

	this->m_ComboBox_NoSuspendTo.GetWindowText(cs);
	if (MyFunction::ToWString(sf.NO_SUSPEND_TO).c_str() != cs)return true;

	return false;
}
bool CEnergySaverDlg::Save()
{
	if (m_Initing)
	{
		//��ʼ���ڼ��ȡ������Ч�����Բ���ִ�б�����
		return true;
	}

	CString cs;
	this->m_Edit_IdelTimeToSuspend.GetWindowText(cs);
	if (!CConfig::SetConfig(TEXT("IDEL_TIME_TO_SUSPEND"), cs))
	{
		ErrorLog(L"д�������ļ�����");
		return false;
	}

	int check_state_lock = this->m_Check_Lock.GetCheck();
	if (BST_CHECKED == check_state_lock)cs = L"TRUE";
	else cs = L"FALSE";
	if (!CConfig::SetConfig(TEXT("IDEL_TIME_LOCK"), cs))
	{
		ErrorLog(L"д�������ļ�����");
		return false;
	}

	int check_state_ip_report = this->m_Check_IP_Report.GetCheck();
	if (BST_CHECKED == check_state_ip_report)cs = L"TRUE";
	else cs = L"FALSE";
	if (!CConfig::SetConfig(L"IP_REPORT", cs))
	{
		ErrorLog(L"д�������ļ�����");
		return false;
	}

	this->m_ComboBox_NoSuspendFrom.GetWindowTextW(cs);
	if (!CConfig::SetConfig(L"NO_SUSPEND_FROM", cs))
	{
		ErrorLog(L"д�������ļ�����");
		return false;
	}

	this->m_ComboBox_NoSuspendTo.GetWindowTextW(cs);
	if (!CConfig::SetConfig(L"NO_SUSPEND_TO", cs))
	{
		ErrorLog(L"д�������ļ�����");
		return false;
	}

	cs.Format(L"%ld", sf.OVERTIME_TO);
	if (!CConfig::SetConfig(L"OVERTIME_TO", cs))
	{
		ErrorLog(L"д�������ļ�����");
		return false;
	}

	sf.init();
	//if(0!=MyFunction::mySystem(L"sc.exe stop \"EnergySaver\""))
	//{
	//	ErrorLog(L"ֹͣ�������");
	//}
	//if(0!=MyFunction::mySystem(L"sc.exe start \"EnergySaver\""))
	//{
	//	ErrorLog(L"�����������");
	//}
	//if(0!=MyFunction::mySystem(L"sc.exe pause \"EnergySaver\""))
	//{
	//	ErrorLog(L"��ͣ�������");
	//}

	return true;
}
void CEnergySaverDlg::ErrorLog(wchar_t const* message)
{
	Log::WriteToLogW(message);
	//MessageBox(message);
}

void CEnergySaverDlg::OnEnChangeEditIdelTimeToSuspend()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CheckChanged())Save();
}


void CEnergySaverDlg::OnCbnSelchangeComboNoSuspendFrom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (CheckChanged())Save();
}


void CEnergySaverDlg::OnCbnSelchangeComboNoSuspendTo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (CheckChanged())Save();
}


void CEnergySaverDlg::OnStnClickedStaticIcon()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CEnergySaverDlg::OnBnClickedButtonLock()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LockWorkStation();
}


void CEnergySaverDlg::OnBnClickedCheckLock()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (CheckChanged())Save();
}

void CEnergySaverDlg::OnNMClickSyslinkWebsite(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	ShellExecuteW(NULL, TEXT("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}
