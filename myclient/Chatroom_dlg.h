#pragma once
#include "afxwin.h"
#include "Resource.h"

// Chatroom_dlg 对话框

class Chatroom_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Chatroom_dlg)

public:
	Chatroom_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Chatroom_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit txt_1;
	CEdit txt_2;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();
	CMFCEditBrowseCtrl path_1;
	afx_msg void OnBnClickedButton3();
};
