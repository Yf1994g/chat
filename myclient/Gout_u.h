#pragma once
#include "afxwin.h"


// CGout_u �Ի���

class CGout_u : public CDialogEx
{
	DECLARE_DYNAMIC(CGout_u)

public:
	CGout_u(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGout_u();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit U_name;
	afx_msg void OnBnClickedOk();
};
