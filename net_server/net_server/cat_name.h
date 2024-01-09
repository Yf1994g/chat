#pragma once
#include "afxwin.h"
#include "socket.h"

// cat_name 对话框

class cat_name : public CDialogEx
{
	DECLARE_DYNAMIC(cat_name)

public:
	cat_name(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~cat_name();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit chat_4;
	afx_msg void OnBnClickedOk();
};
