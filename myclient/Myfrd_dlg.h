#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "Chatroom_dlg.h"


// Myfrd_dlg �Ի���

#define WM_MY_CUSTOM_MESSAGE (WM_USER + 100) //�����Ի������Ϣ

class Myfrd_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Myfrd_dlg)

public:
	Myfrd_dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Myfrd_dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit frd_list;
	CEdit frd_name;
	afx_msg void OnBnClickedButton3();
	afx_msg LRESULT OnCreateDlg(WPARAM,LPARAM);//��Ϣ��������
	afx_msg void OnBnClickedButton2();
	CButton S_button;
	afx_msg void OnBnClickedButton1();
};