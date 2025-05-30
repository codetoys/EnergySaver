
// EnergySaverDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "../include/ServiceFunction.h"

// CEnergySaverDlg 对话框
class CEnergySaverDlg : public CDialogEx
{
// 构造
public:
	CEnergySaverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ENERGYSAVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 服务状态
	CEdit m_Edit_ServiceState;
	// 空闲此时间后休眠
	CEdit m_Edit_IdelTimeToSuspend;
	CComboBox m_ComboBox_NoSuspendFrom;
	CComboBox m_ComboBox_NoSuspendTo;
	CEdit m_Edit_WorkOvertime;
	// 加班状态
	CEdit m_Edit_OvertimeState;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonWorkOvertime();
	afx_msg void OnBnClickedButtonWorkOvertimeCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnMyExit(WPARAM,LPARAM);

	CEdit m_edit_last;

	CRect rt;//启动时的窗口位置
	bool isStartHide;//启动时隐藏窗口
	ServiceFunction sf;
	bool m_Initing;//正在启动
	bool m_ServiceInstalled = true;//服务是否已安装
	DWORD GetServiceState();
	void ShowOvertimeState();
	void ShowServiceState();
	bool CheckChanged();
	bool Save();
	void ErrorLog(wchar_t const * message);
	afx_msg void OnEnChangeEditIdelTimeToSuspend();
	afx_msg void OnCbnSelchangeComboNoSuspendFrom();
	afx_msg void OnCbnSelchangeComboNoSuspendTo();
	afx_msg void OnStnClickedStaticIcon();
	CStatic m_static_op;
	afx_msg void OnBnClickedButtonLock();
	CButton m_Check_Lock;
	afx_msg void OnBnClickedCheckLock();
	CButton m_Check_IP_Report;
	CLinkCtrl m_SysLink_Website;
	afx_msg void OnNMClickSyslinkWebsite(NMHDR* pNMHDR, LRESULT* pResult);
};
