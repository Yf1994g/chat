#pragma once
#include "afxwin.h"
#include "socket.h"

// cat_name �Ի���

class cat_name : public CDialogEx
{
	DECLARE_DYNAMIC(cat_name)

public:
	cat_name(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~cat_name();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit chat_4;
	afx_msg void OnBnClickedOk();
};
