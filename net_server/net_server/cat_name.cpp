// cat_name.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "net_server.h"
#include "cat_name.h"
#include "afxdialogex.h"
#include "socket.h"


// cat_name �Ի���

IMPLEMENT_DYNAMIC(cat_name, CDialogEx)

cat_name::cat_name(CWnd* pParent /*=NULL*/)
	: CDialogEx(cat_name::IDD, pParent)
{

}

cat_name::~cat_name()
{
}

void cat_name::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, chat_4);
}


BEGIN_MESSAGE_MAP(cat_name, CDialogEx)
	ON_BN_CLICKED(IDOK, &cat_name::OnBnClickedOk)
END_MESSAGE_MAP()


// cat_name ��Ϣ�������


void cat_name::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sent_t;
	char *buffer = new char[1024];

	chat_4.GetWindowTextA(sent_t);

	strcpy(buffer, sent_t.GetString());

	Gout_user(buffer);

	delete[] buffer;
	CDialogEx::OnOK();
}
