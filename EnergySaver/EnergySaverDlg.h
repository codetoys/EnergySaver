
// EnergySaverDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "../include/ServiceFunction.h"

// CEnergySaverDlg �Ի���
class CEnergySaverDlg : public CDialogEx
{
// ����
public:
	CEnergySaverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ENERGYSAVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ����״̬
	CEdit m_Edit_ServiceState;
	// ���д�ʱ�������
	CEdit m_Edit_IdelTimeToSuspend;
	CComboBox m_ComboBox_NoSuspendFrom;
	CComboBox m_ComboBox_NoSuspendTo;
	CEdit m_Edit_WorkOvertime;
	// �Ӱ�״̬
	CEdit m_Edit_OvertimeState;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonWorkOvertime();
	afx_msg void OnBnClickedButtonWorkOvertimeCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnMyExit(WPARAM,LPARAM);

	CEdit m_edit_last;

	CRect rt;//����ʱ�Ĵ���λ��
	bool isStartHide;//����ʱ���ش���
	ServiceFunction sf;
	bool m_Initing;//��������
	bool m_ServiceInstalled = true;//�����Ƿ��Ѱ�װ
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
