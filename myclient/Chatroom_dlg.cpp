// Chatroom_dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "myclient.h"
#include "Chatroom_dlg.h"
#include "afxdialogex.h"
#include "mysocket.h"
#include <fstream>


// Chatroom_dlg �Ի���

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


// Chatroom_dlg ��Ϣ�������


void Chatroom_dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString Gettxt;
	char Thome[100] = {0};
	Chatroom_dlg * dlg = this;

	txt_2.GetWindowTextA(Gettxt);
	const char* cstr = Gettxt.GetString();

	SentData(cstr, dlg);

}


void Chatroom_dlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Chatroom_dlg * dlg = this;

	ReleaseP(dlg);

	CDialogEx::OnClose();
}


void Chatroom_dlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString Gettxt;

	txt_2.GetWindowTextA(Gettxt);
	const char* cstr = Gettxt.GetString();

	std::ofstream file("output.txt");  // ��������ļ���

	if (file.is_open()) {  // ����ļ��Ƿ�ɹ���
		file << cstr;     // ���ַ���д���ļ�
		file.close();     // �ر��ļ���
	}
	MessageBox(_T("����ɹ�!"));
}


void Chatroom_dlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString path;
	Chatroom_dlg* dlg = this;

	path_1.GetWindowTextA(path);
	const char* cstr = path.GetString();
	MessageBox(path);

	SentFileToServer(cstr, dlg);
}
