// Gout_u.cpp : 实现文件
//

#include "stdafx.h"
#include "myclient.h"
#include "Gout_u.h"
#include "afxdialogex.h"
#include "mysocket.h"

// CGout_u 对话框

IMPLEMENT_DYNAMIC(CGout_u, CDialogEx)

CGout_u::CGout_u(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGout_u::IDD, pParent)
{

}

CGout_u::~CGout_u()
{
}

void CGout_u::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, U_name);
}


BEGIN_MESSAGE_MAP(CGout_u, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGout_u::OnBnClickedOk)
END_MESSAGE_MAP()


// CGout_u 消息处理程序


void CGout_u::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString data;
	char name[100];
	U_name.GetWindowTextA(data);
	const char* cstr = data.GetString();

	sprintf(name,"%s",cstr);
	Gout_user(name);
	CDialogEx::OnOK();
}
