#pragma once
#include "afxwin.h"


// CGout_u 对话框

class CGout_u : public CDialogEx
{
	DECLARE_DYNAMIC(CGout_u)

public:
	CGout_u(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGout_u();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit U_name;
	afx_msg void OnBnClickedOk();
};
