#include "StdAfx.h"
#include "mysocket.h"
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_SOCKS 64

SOCKET clientSocket = -1;
char Client_name[100] = {0};

struct DLG_INFO user_dlg[MAX_SOCKS];

int ReleaseP(Chatroom_dlg * dlg)
{
	int i;

	for (i =0 ;i < MAX_SOCKS; i++) {
		if (user_dlg[i].Wroom == dlg) {
			user_dlg[i].Wroom = NULL;
			memset(user_dlg[i].name,0, 100);
		}
	}
	return 0;
}

int CreatChatDlg(char *data, Myfrd_dlg *dlg)
{
	Chatroom_dlg *chat_1 = new Chatroom_dlg;
	int i=0;

	if (strcmp(data, "public") == 0) {
		memcpy(user_dlg[0].name, data, strlen(data));
		user_dlg[0].Wroom = chat_1;
	} else {
		for(i = 1; i< MAX_SOCKS; i++) {
			if (user_dlg[i].Wroom == NULL) {
				memcpy(user_dlg[i].name, data, strlen(data));
				user_dlg[i].Wroom = chat_1;
				break;
			}
		}
	}

	dlg->SendMessage(WM_MY_CUSTOM_MESSAGE,(WPARAM)chat_1);//发送建立非模态对话框消息
	return 0;
}

static int SentClientinfo()
{
	struct CHAT_INFOR chat_info;
	int len;

	chat_info.typ = 2;
	memcpy(chat_info.Iname, Client_name, 100);
	
	len = send(clientSocket, (char *)&chat_info, sizeof(CHAT_INFOR), 0);

    if ( len == SOCKET_ERROR) {
        TRACE(_T("Failed to send data to server"));
    }
	return 0;
}

int Link_net(const char* host, int port)
{
	// 初始化 Winsock

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return -1;
    }

    // 创建客户端 socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        return -1;
    }

    // 设置服务器地址
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 连接服务器
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

	SentClientinfo();
	return 0;
}

static int SetchatToPublicRoom(char *buffer,struct CHAT_INFOR *chat_info, Myfrd_dlg *dlg)
{
	CString set_txt;
	char dataout[4096] = {0};
	char tname[100];

	sprintf(dataout, "%s:%s",chat_info->Iname,buffer);

	if (user_dlg[0].Wroom == NULL) {
		sprintf(tname,"public");
		CreatChatDlg(tname, dlg);
	}
	set_txt.Format(dataout);
	user_dlg[0].Wroom->txt_1.SetSel(-1);
	user_dlg[0].Wroom->txt_1.ReplaceSel(set_txt);

	return 0;
}

static int SetSideroom(char *buffer,struct CHAT_INFOR *chat_info, Myfrd_dlg *dlg)
{
	int i;
	CString set_txt;
	char dataout[4096] = {0};

	sprintf(dataout, "%s:%s",chat_info->Iname,buffer);
	for (i=1;i<MAX_SOCKS; i++) {
		TRACE("user_dlg  %s send_name %s\r\n", user_dlg[i].name , chat_info->Iname);
		if (strcmp(user_dlg[i].name, chat_info->Iname) == 0) {
			set_txt.Format(dataout);
			user_dlg[i].Wroom->txt_1.SetSel(-1);
			user_dlg[i].Wroom->txt_1.ReplaceSel(set_txt);
			break;
		} else {
			CreatChatDlg(chat_info->Iname, dlg);
			set_txt.Format(dataout);
			user_dlg[i].Wroom->txt_1.SetSel(-1);
			user_dlg[i].Wroom->txt_1.ReplaceSel(set_txt);
			break;
		}
	}

	return 0;
}

static int RecvFile(int filesize, char * name)
{
	char outdata[4096] = { 0 };
	char buffer[4096];
	int recvBytes;
	FILE* outFile;
	ULONGLONG totalRecvBytes = 0;
	ULONGLONG fileSize = 0;
	int bytesToRecv;

	// 打开输出文件流
	fileSize = filesize;
	sprintf(outdata, "%s", name);
	outFile = fopen(outdata, "wb");
	if (outFile == NULL) {
		return -1;
	}

	while (totalRecvBytes < fileSize) {
		bytesToRecv = static_cast<int>(min(4096, fileSize - totalRecvBytes));
		recvBytes = recv(clientSocket, buffer, bytesToRecv, 0);
		if (recvBytes <= 0) {
			return -1;
		}

		fwrite(buffer, sizeof(char), recvBytes, outFile);
		totalRecvBytes += recvBytes;
	}
	fclose(outFile);

	return 0;
}

static int RecvFiletoHome(struct Data_info* data_info, struct CHAT_INFOR* chat_info, Myfrd_dlg* dlg)
{
	CString set_txt;
	char dataout[4096] = { 0 };
	char tname[100];

	RecvFile(data_info->file_size, data_info->name);

	sprintf(dataout, "接收到%s的文件 %s ", chat_info->Iname, data_info->name);

	if (user_dlg[0].Wroom == NULL) {
		sprintf(tname, "public");
		CreatChatDlg(tname, dlg);
	}
	set_txt.Format(dataout);
	user_dlg[0].Wroom->txt_1.SetSel(-1);
	user_dlg[0].Wroom->txt_1.ReplaceSel(set_txt);

	return 0;
}

static int RecvFiletoUser(struct Data_info* data_info, struct CHAT_INFOR* chat_info, Myfrd_dlg* dlg)
{
	CString set_txt;
	char dataout[4096] = { 0 };
	char tname[100];

	RecvFile(data_info->file_size, data_info->name);

	sprintf(dataout, "接收到%s的文件 %s ", chat_info->Iname, data_info->name);

	if (user_dlg[0].Wroom == NULL) {
		sprintf(tname, "public");
		CreatChatDlg(tname, dlg);
	}
	set_txt.Format(dataout);
	user_dlg[0].Wroom->txt_1.SetSel(-1);
	user_dlg[0].Wroom->txt_1.ReplaceSel(set_txt);

	return 0;
}

int RecverData(Myfrd_dlg *dlg)
{
	struct CHAT_INFOR chat_info;
	int recv_len;
	char buffer[4096]= {0};
	int bytesRecv;
	struct Data_info data_info;

	Sleep(1000);
	if ((strcmp(Client_name, "admin") == 0))
		dlg->S_button.ShowWindow(SW_SHOW);
	while(true) {
		recv_len = recv(clientSocket, (char *)&chat_info, sizeof(chat_info), 0);
		if(recv_len < 0)
			break;

		switch(chat_info.typ) {
		case 1:
			TRACE(_T("SetchatToPublicRoom to server"));
			memset(buffer, 0, 4096);
			bytesRecv = recv(clientSocket, buffer, chat_info.len, 0);
			TRACE("SetchatToPublicRoom %s", buffer);
			SetchatToPublicRoom(buffer, &chat_info, dlg);
			break;
		case 2:
			memset(buffer, 0, 4096);
			bytesRecv = recv(clientSocket, buffer, chat_info.len, 0);
			TRACE("list %s  bytesRecv %d",buffer, bytesRecv);
			dlg->frd_list.SetWindowTextA(_T(buffer));
			break;
		case 3:
			memset(buffer, 0, 4096);
			bytesRecv = recv(clientSocket, buffer, chat_info.len, 0);
			SetSideroom(buffer, &chat_info, dlg);
			break;
		case 5:
			TRACE(_T("RecvFiletoHome !!!"));
			memset(buffer, 0, 4096);
			bytesRecv = recv(clientSocket, (char*)&data_info, sizeof(Data_info), 0);
			TRACE(_T("data_info %d", data_info.file_size));
			RecvFiletoHome(&data_info, &chat_info, dlg);
			break;
		case 6:
			TRACE(_T("RecvFiletoUser !!!"));
			memset(buffer, 0, 4096);
			bytesRecv = recv(clientSocket, (char*)&data_info, sizeof(Data_info), 0);
			RecvFiletoUser(&data_info, &chat_info, dlg);
			break;
		default:break;
		}

	}
	return 0;
}

int SentData(const char *data, Chatroom_dlg * dlg)
{
	struct CHAT_INFOR chat_info;
	char buffer[1024] = {0};
	int len;
	int i;
	CString set_txt;
	char dataout[4096] = { 0 };

	len = sprintf(buffer,"%s\r\n",data);
	TRACE("buf %s  ",buffer);
	if (user_dlg[0].Wroom == dlg) {
		chat_info.typ = 1;
		chat_info.len = len;
		memcpy(chat_info.Iname, Client_name, 100);
	
		send(clientSocket, (char *)&chat_info, sizeof(CHAT_INFOR), 0);
		send(clientSocket, buffer, len, 0);
	} else {
		for (i = 1; i < MAX_SOCKS; i++) {
			if (user_dlg[i].Wroom == dlg) {
				chat_info.typ = 3;
				chat_info.len = len;
				memcpy(chat_info.Iname, Client_name, 100);
				memcpy(chat_info.send_name, user_dlg[i].name, 100);

				sprintf(dataout, "%s:%s", Client_name, buffer);
				set_txt.Format(dataout);
				dlg->txt_1.SetSel(-1);
				dlg->txt_1.ReplaceSel(set_txt);

				send(clientSocket, (char *)&chat_info, sizeof(CHAT_INFOR), 0);
				send(clientSocket, buffer, len, 0);
				break;
			}
		}
	}

    if ( len == SOCKET_ERROR) {
        TRACE(_T("Failed to send data to server"));
    }
	return 0;
}

int SetC_name(const char *data)
{
	sprintf(Client_name,"%s", data);
	TRACE("Client_name %s", Client_name);
	return 0;
}

int Gout_user(const char *name)
{
	struct CHAT_INFOR chat_info;

	chat_info.typ = 4;
	memcpy(chat_info.send_name, name, 100);

	send(clientSocket, (char *)&chat_info, sizeof(CHAT_INFOR), 0);
	return 0;
}

void GetFileNameFromPath(const char* filePath, char* fileName, size_t maxSize) {
	const char* lastSeparator = strrchr(filePath, '\\');
	if (lastSeparator != NULL) {
		strncpy(fileName, lastSeparator + 1, maxSize - 1);
	}
	else {
		strncpy(fileName, filePath, maxSize - 1);
	}
	fileName[maxSize - 1] = '\0';
}

int SentFileToServer(const char* file_path, Chatroom_dlg* dlg)
{
	struct CHAT_INFOR chat_info;

	char szBuffer[4096];
	struct Data_info data_info;
	int nRead;
	int i;

	if (user_dlg[0].Wroom == dlg) {
		chat_info.typ = 5;
		memcpy(chat_info.Iname, Client_name, 100);

		send(clientSocket, (char*)&chat_info, sizeof(CHAT_INFOR), 0);
	}
	else {
		for (i = 1; i < MAX_SOCKS; i++) {
			if (user_dlg[i].Wroom == dlg) {
				chat_info.typ = 6;
				memcpy(chat_info.Iname, Client_name, 100);
				memcpy(chat_info.send_name, user_dlg[i].name, 100);
				send(clientSocket, (char*)&chat_info, sizeof(CHAT_INFOR), 0);
				break;
			}
		}
	}

	GetFileNameFromPath(file_path, data_info.name, 100);

	CFile file(file_path, CFile::modeRead | CFile::typeBinary);
	data_info.file_size = file.GetLength();

	send(clientSocket, (char*)&data_info, sizeof(Data_info), 0);
	while ((nRead = file.Read(szBuffer, 4096)) > 0) {
		// 发送数据
		send(clientSocket, szBuffer, nRead, 0);
	}

	// 关闭文件
	file.Close();

	return 0;
}