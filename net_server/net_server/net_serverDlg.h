
// net_serverDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "resource.h"

// Cnet_serverDlg 对话框
class Cnet_serverDlg : public CDialogEx
{
// 构造
public:
	Cnet_serverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NET_SERVER_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	CEdit chat_1;
	CEdit chat_2;
	CEdit chat_3;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCancel();
	CEdit chat_4;
};
