
// net_serverDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "resource.h"

// Cnet_serverDlg �Ի���
class Cnet_serverDlg : public CDialogEx
{
// ����
public:
	Cnet_serverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NET_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
