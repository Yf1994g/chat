
// net_server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cnet_serverApp:
// �йش����ʵ�֣������ net_server.cpp
//

class Cnet_serverApp : public CWinApp
{
public:
	Cnet_serverApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cnet_serverApp theApp;