// Chatroom_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "myclient.h"
#include "Chatroom_dlg.h"
#include "afxdialogex.h"
#include "mysocket.h"
#include <fstream>


// Chatroom_dlg 对话框

IMPLEMENT_DYNAMIC(Chatroom_dlg, CDialogEx)

Chatroom_dlg::Chatroom_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Chatroom_dlg::IDD, pParent)
{

}

Chatroom_dlg::~Chatroom_dlg()
{
}

void Chatroom_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, txt_1);
	DDX_Control(pDX, IDC_EDIT2, txt_2);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, path_1);
}


BEGIN_MESSAGE_MAP(Chatroom_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Chatroom_dlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &Chatroom_dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Chatroom_dlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// Chatroom_dlg 消息处理程序


void Chatroom_dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Gettxt;
	char Thome[100] = {0};
	Chatroom_dlg * dlg = this;

	txt_2.GetWindowTextA(Gettxt);
	const char* cstr = Gettxt.GetString();

	SentData(cstr, dlg);

}


void Chatroom_dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Chatroom_dlg * dlg = this;

	ReleaseP(dlg);

	CDialogEx::OnClose();
}


void Chatroom_dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Gettxt;

	txt_2.GetWindowTextA(Gettxt);
	const char* cstr = Gettxt.GetString();

	std::ofstream file("output.txt");  // 创建输出文件流

	if (file.is_open()) {  // 检查文件是否成功打开
		file << cstr;     // 将字符串写入文件
		file.close();     // 关闭文件流
	}
	MessageBox(_T("保存成功!"));
}


void Chatroom_dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	Chatroom_dlg* dlg = this;

	path_1.GetWindowTextA(path);
	const char* cstr = path.GetString();
	MessageBox(path);

	SentFileToServer(cstr, dlg);
}
