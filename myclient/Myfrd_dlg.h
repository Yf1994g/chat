#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "Chatroom_dlg.h"


// Myfrd_dlg 对话框

#define WM_MY_CUSTOM_MESSAGE (WM_USER + 100) //创建对话框的消息

class Myfrd_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Myfrd_dlg)

public:
	Myfrd_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Myfrd_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit frd_list;
	CEdit frd_name;
	afx_msg void OnBnClickedButton3();
	afx_msg LRESULT OnCreateDlg(WPARAM,LPARAM);//消息函数声明
	afx_msg void OnBnClickedButton2();
	CButton S_button;
	afx_msg void OnBnClickedButton1();
};