
// EnergySaverDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// CEnergySaverDlg 对话框




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


// CEnergySaverDlg 消息处理程序

BOOL CEnergySaverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//使窗口不在任务栏出现
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	//首先处理参数，如果有-auto则隐藏窗口
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
					GetWindowRect(&rt);//由于此时窗口尚未显示，获得的位置在（0,0），后续显示时需要用CenterWindow移动到中心
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
		this->MessageBox(TEXT("初始化失败"));
	}

	//预设下拉框内容
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

	this->m_static_op.SetWindowText(CEnergyManager::isShutdown() ? TEXT("关机") : TEXT("休眠"));

	SetTimer(1, SLEEP_TIME, 0);

	m_Initing = false;
	ShowOvertimeState();
	ShowServiceState();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEnergySaverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEnergySaverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEnergySaverDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		if (isStartHide)
		{
			isStartHide = false;

			ShowWindow(SW_HIDE);//隐藏
			MoveWindow(rt);//恢复位置
			CenterWindow();//移动到中心
		}

		ShowOvertimeState();
		ShowServiceState();

		time_t t1 = time(NULL);
		CEnergyManager em;
		ULONGLONG n = em.GetLastInputPassed();
		sf.setLastInputToFile(t1);//记录到文件以便服务能够获得,实际记录的是当前时间，服务根据这个休眠

		bool soon_to_suspend;
		int time_to_suspend;
		int after_suspend;
		if (sf.doServiceFunction(n, soon_to_suspend, time_to_suspend, after_suspend))
		{
			ShowWindow(SW_HIDE);//睡眠后要隐藏窗口，以免恢复系统后仍在显示
		}
		else
		{
			if (n > sf.IDEL_TIME_TO_SUSPEND && BST_CHECKED == m_Check_Lock.GetCheck())
			{
				OnBnClickedButtonLock();//立即锁屏
			}
		}

		CString str;
		if (soon_to_suspend)
		{
			//呼出屏保，如果未设置屏保，系统会启动登录界面
			::PostMessage(GetDesktopWindow()->GetSafeHwnd(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);
			//ShowWindow(SW_SHOW);
			//SetForegroundWindow();
		}
		if (time_to_suspend > 0 || after_suspend > 0)
		{
			str.Format(TEXT("%d 秒后休眠 %d"), time_to_suspend, after_suspend);
		}
		else
		{
			str.Format(TEXT("上次动作于 %lld 秒前"), n);
		}
		m_edit_last.SetWindowText(str);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CEnergySaverDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(L"OnBnClickedOk");
	if (IDYES == AfxMessageBox(TEXT("退出程序将不能实现节能减排，确认要退出吗？"), MB_YESNO | MB_ICONEXCLAMATION))
	{
		CDialogEx::OnOK();
	}
}


void CEnergySaverDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	sf.OVERTIME_TO = 0;
	ShowOvertimeState();
	Save();
}
void CEnergySaverDlg::ShowOvertimeState()
{
	time_t t1 = time(NULL);
	if (t1 >= sf.OVERTIME_TO)
	{
		this->m_Edit_OvertimeState.SetWindowText(TEXT("非加班中"));
	}
	else
	{
		CString str;
		time_t remain = sf.OVERTIME_TO - t1;
		str.Format(TEXT("加班中，剩余%lld小时%lld分钟"), remain / 3600, (remain % 3600) / 60);
		this->m_Edit_OvertimeState.SetWindowText(str);
	}
}
DWORD CEnergySaverDlg::GetServiceState()
{
	//查询服务状态
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
				this->m_Edit_ServiceState.SetWindowText(TEXT("QueryServiceStatus失败"));
			}
			else
			{
				return status.dwCurrentState;
			}
		}
		else
		{
			this->m_Edit_ServiceState.SetWindowText(TEXT("OpenService失败"));
		}
	}
	else
	{
		this->m_Edit_ServiceState.SetWindowText(TEXT("OpenSCManager失败"));
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
		this->m_Edit_ServiceState.SetWindowText(TEXT("正常"));
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
		this->m_SysLink_Website.SetWindowTextW(TEXT("<a href=\"https://codetoys.github.io/EnergySaver/\">服务已安装</a>"));
	}
	else
	{
		this->m_SysLink_Website.SetWindowTextW(TEXT("<a href=\"https://codetoys.github.io/EnergySaver/\">服务未安装，单击打开网站</a>"));
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
		//初始化期间获取到的无效，所以不能执行本功能
		return true;
	}

	CString cs;
	this->m_Edit_IdelTimeToSuspend.GetWindowText(cs);
	if (!CConfig::SetConfig(TEXT("IDEL_TIME_TO_SUSPEND"), cs))
	{
		ErrorLog(L"写入配置文件出错");
		return false;
	}

	int check_state_lock = this->m_Check_Lock.GetCheck();
	if (BST_CHECKED == check_state_lock)cs = L"TRUE";
	else cs = L"FALSE";
	if (!CConfig::SetConfig(TEXT("IDEL_TIME_LOCK"), cs))
	{
		ErrorLog(L"写入配置文件出错");
		return false;
	}

	int check_state_ip_report = this->m_Check_IP_Report.GetCheck();
	if (BST_CHECKED == check_state_ip_report)cs = L"TRUE";
	else cs = L"FALSE";
	if (!CConfig::SetConfig(L"IP_REPORT", cs))
	{
		ErrorLog(L"写入配置文件出错");
		return false;
	}

	this->m_ComboBox_NoSuspendFrom.GetWindowTextW(cs);
	if (!CConfig::SetConfig(L"NO_SUSPEND_FROM", cs))
	{
		ErrorLog(L"写入配置文件出错");
		return false;
	}

	this->m_ComboBox_NoSuspendTo.GetWindowTextW(cs);
	if (!CConfig::SetConfig(L"NO_SUSPEND_TO", cs))
	{
		ErrorLog(L"写入配置文件出错");
		return false;
	}

	cs.Format(L"%ld", sf.OVERTIME_TO);
	if (!CConfig::SetConfig(L"OVERTIME_TO", cs))
	{
		ErrorLog(L"写入配置文件出错");
		return false;
	}

	sf.init();
	//if(0!=MyFunction::mySystem(L"sc.exe stop \"EnergySaver\""))
	//{
	//	ErrorLog(L"停止服务出错");
	//}
	//if(0!=MyFunction::mySystem(L"sc.exe start \"EnergySaver\""))
	//{
	//	ErrorLog(L"启动服务出错");
	//}
	//if(0!=MyFunction::mySystem(L"sc.exe pause \"EnergySaver\""))
	//{
	//	ErrorLog(L"暂停服务出错");
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (CheckChanged())Save();
}


void CEnergySaverDlg::OnCbnSelchangeComboNoSuspendFrom()
{
	// TODO: 在此添加控件通知处理程序代码
	if (CheckChanged())Save();
}


void CEnergySaverDlg::OnCbnSelchangeComboNoSuspendTo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (CheckChanged())Save();
}


void CEnergySaverDlg::OnStnClickedStaticIcon()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CEnergySaverDlg::OnBnClickedButtonLock()
{
	// TODO: 在此添加控件通知处理程序代码
	LockWorkStation();
}


void CEnergySaverDlg::OnBnClickedCheckLock()
{
	// TODO: 在此添加控件通知处理程序代码
	if (CheckChanged())Save();
}

void CEnergySaverDlg::OnNMClickSyslinkWebsite(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	ShellExecuteW(NULL, TEXT("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}
