
// EnergySaver.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEnergySaverApp:
// �йش����ʵ�֣������ EnergySaver.cpp
//

class CEnergySaverApp : public CWinApp
{
public:
	CEnergySaverApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEnergySaverApp theApp;
