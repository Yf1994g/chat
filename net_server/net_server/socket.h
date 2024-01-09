#ifndef _MY_SOCKET_H
#define _MY_SOCKET_H


#include "net_serverDlg.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

struct CHAT_INFOR {
	int typ;
	int len;
	char Iname[100];
	char send_name[100];
};

struct Data_info {
	int typ;
	int buf_len;
	char name[100];
	ULONGLONG file_size;
};

int creat_chat_room(Cnet_serverDlg* dlg, const char* host, int port);

int close_socket();

int Gout_user(char* name);

int SentUserInfo(Cnet_serverDlg* dlg);

#endif
