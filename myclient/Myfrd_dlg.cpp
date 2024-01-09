// Myfrd_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "myclient.h"
#include "Myfrd_dlg.h"
#include "afxdialogex.h"
#include "mysocket.h"
#include "Gout_u.h"

// Myfrd_dlg 对话框

IMPLEMENT_DYNAMIC(Myfrd_dlg, CDialogEx)

Myfrd_dlg::Myfrd_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Myfrd_dlg::IDD, pParent)
{

}

Myfrd_dlg::~Myfrd_dlg()
{
}

void Myfrd_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, frd_list);
	DDX_Control(pDX, IDC_EDIT2, frd_name);
	DDX_Control(pDX, IDC_BUTTON1, S_button);
}


BEGIN_MESSAGE_MAP(Myfrd_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &Myfrd_dlg::OnBnClickedButton3)
	ON_MESSAGE(WM_MY_CUSTOM_MESSAGE,OnCreateDlg)//消息映射
	ON_BN_CLICKED(IDC_BUTTON2, &Myfrd_dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &Myfrd_dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Myfrd_dlg 消息处理程序


void Myfrd_dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	char tname[100];
	Myfrd_dlg *dlg= this;

	sprintf(tname,"public");
	CreatChatDlg(tname, dlg);
}

LRESULT Myfrd_dlg::OnCreateDlg(WPARAM wParam,LPARAM lParam )
{
         Chatroom_dlg* p=(Chatroom_dlg*)wParam;  //CMyDlg为非模态对话框类
         p->Create(IDD_DIALOG2,this);  //IDD_MY_DLG为非模态对话框资源ID
         p->ShowWindow(SW_SHOW);
		 return 0;
}

void Myfrd_dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Fname;
	char tname[100];
	Myfrd_dlg *dlg= this;

	frd_name.GetWindowTextA(Fname);

	if (Fname.GetLength() == 0){
		MessageBox(_T("输入昵称!"));
		return;
	}
	const char* cstr = Fname.GetString();

	sprintf(tname,"%s", cstr);
	CreatChatDlg(tname, dlg);
}

void Myfrd_dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CGout_u dlg;

	dlg.DoModal();
}
