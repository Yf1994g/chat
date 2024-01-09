// sever.cpp : 定义控制台应用程序的入口点。
//
#include "StdAfx.h"
#include <iostream>
#include <Windows.h>
#include "net_serverDlg.h"
#include "socket.h"
#include <direct.h>
#include <io.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_MYCLIENTS 10
#define DATA_MAX 4096
#define MAX_SOCKS 64

struct socket_infor {
    SOCKET socket;
    Cnet_serverDlg* dlg;
};

WSAPOLLFD pollFd[64];
char UserNamelist[64][100];

int working_flag = 1;

SOCKET creat_socket(Cnet_serverDlg* dlg, const char *host, int port)
{
    WSADATA wsaData;
    int result;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        return -1;
    }

    // 创建 socket
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // 绑定地址和端口号
    struct sockaddr_in localAddr;
    ZeroMemory(&localAddr, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, host, &(localAddr.sin_addr));

    result = bind(listenSock, (SOCKADDR*)&localAddr, sizeof(localAddr));
    if (result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    // 监听端口号
    result = listen(listenSock, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    return listenSock;
}

int SentToPublicRoom(char* buffer, CHAT_INFOR* chat_info)
{
    struct CHAT_INFOR send_info;

    send_info.typ = 1;
    send_info.len = chat_info->len;
    memcpy(send_info.Iname, chat_info->Iname, 100);
    for (int i = 1; i < MAX_SOCKS; ++i) {
        if (pollFd[i].fd != 0) {
            send(pollFd[i].fd, (char*)&send_info, sizeof(CHAT_INFOR), 0);
            send(pollFd[i].fd, buffer, strlen(buffer), 0);
        }
    }

    return 0;
}

int SentUserInfo(Cnet_serverDlg* dlg)
{
    struct CHAT_INFOR send_info;
    char namedata[4096] = { 0 };
    int len = 0;
    int total_len = 0;
    CString set_t;

    for (int i = 1; i < MAX_SOCKS; ++i) {
        if (pollFd[i].fd != 0) {
            len = sprintf(namedata + total_len, "%s\r\n", UserNamelist[i]);
            total_len += len;
        }
    }


    set_t.Format(namedata);
    dlg->chat_3.SetWindowTextA(set_t);

    send_info.typ = 2;
    send_info.len = total_len;
    for (int i = 1; i < MAX_SOCKS; ++i) {
        if (pollFd[i].fd != 0) {
            send(pollFd[i].fd, (char*)&send_info, sizeof(CHAT_INFOR), 0);
            send(pollFd[i].fd, namedata, total_len, 0);
        }
    }

    return 0;
}

static int SentToSideRoom(char* buffer, CHAT_INFOR* chat_info)
{
    struct CHAT_INFOR send_info;

    send_info.typ = 3;
    send_info.len = chat_info->len;
    memcpy(send_info.Iname, chat_info->Iname, 100);
    memcpy(send_info.send_name, chat_info->send_name, 100);
    printf("sent  %s", buffer);
    for (int i = 1; i < MAX_SOCKS; ++i) {
        if (strcmp(UserNamelist[i], send_info.send_name) == 0) {
            send(pollFd[i].fd, (char*)&send_info, sizeof(CHAT_INFOR), 0);
            send(pollFd[i].fd, buffer, strlen(buffer), 0);
        }
    }

    return 0;
}

int Gout_user(char* name)
{
    int i;

    for (i = 1; i < MAX_SOCKS; i++) {
        if (strcmp(UserNamelist[i], name) == 0) {
            closesocket(pollFd[i].fd);
            pollFd[i].fd = 0;
            pollFd[i].events = 0;
        }
    }

    return 0;
}

static int RecvFiletransfer(char* data, SOCKET Sock)
{
    struct Data_info* data_info;
    ULONGLONG fileSize = 0;
    char outdata[4096] = { 0 };
    char buffer[4096] = { 0 };
    FILE* outFile;
    ULONGLONG totalRecvBytes = 0;
    int bytesToRecv;
    int recvBytes;


    data_info = (struct Data_info*)data;

    fileSize = data_info->file_size;
    sprintf(outdata, "%s", data_info->name);
    outFile = fopen(outdata, "wb");
    if (outFile == NULL) {
        TRACE("打开文件失败！\r\n");
        return 1;
    }
    TRACE("打开文件成功！\r\n");
    while (totalRecvBytes < data_info->file_size) {
        bytesToRecv = static_cast<int>(min(4096, fileSize - totalRecvBytes));
        recvBytes = recv(Sock, buffer, bytesToRecv, 0);
        if (recvBytes <= 0) {
            std::cerr << "Failed to receive file content.\n";
            break;
        }

        fwrite(buffer, sizeof(char), recvBytes, outFile);
        totalRecvBytes += recvBytes;
    }
    fclose(outFile);
    return 0;
}

static int SentFileTohome(char* data, CHAT_INFOR* chat_info)
{
    struct Data_info* data_info;
    char szBuffer[4096];
    char outdata[4096] = { 0 };
    int nRead;
    FILE* outFile;
    long fileSize;

    struct Data_info file_data = {0};
    struct CHAT_INFOR send_info;

    data_info = (struct Data_info*)data;
    file_data.file_size = data_info->file_size;
    memcpy(file_data.name, data_info->name, strlen(data_info->name));
    sprintf(outdata, "%s", data_info->name);
    outFile = fopen(outdata, "rb");
    if (outFile < 0) {
        TRACE("打开文件失败！\r\n");
        return 1;
    }


    TRACE("打开文件成功！\r\n");
    send_info.typ = 5;
    memcpy(send_info.Iname, chat_info->Iname, 100);
    for (int i = 1; i < MAX_SOCKS; ++i) {
        if (pollFd[i].fd != 0) {
            send(pollFd[i].fd, (char*)&send_info, sizeof(CHAT_INFOR), 0);
            send(pollFd[i].fd, (char*)&file_data, sizeof(Data_info), 0);
            while ((nRead = fread(szBuffer, sizeof(char), 4096, outFile)) > 0) {
                    // 发送数据
                send(pollFd[i].fd, szBuffer, nRead, 0);
            }
        }
    }
    TRACE("发送文件 %s \r\n", file_data.name);
    // 关闭文件
    fclose(outFile);

    return 0;
}

static int SentFileToUser(char* data, CHAT_INFOR* chat_info)
{
    struct Data_info* data_info;
    char szBuffer[4096];
    char outdata[4096] = { 0 };
    int nRead;
    FILE* outFile;
    long fileSize;

    struct CHAT_INFOR send_info;
    struct Data_info file_data = { 0 };

    data_info = (struct Data_info*)data;
    file_data.file_size = data_info->file_size;
    memcpy(file_data.name, data_info->name, strlen(data_info->name));

    sprintf(outdata, "%s", data_info->name);
    outFile = fopen(outdata, "rb");
    if (outFile < 0) {
        TRACE("打开文件失败！\r\n");
        return 1;
    }

    send_info.typ = 6;
    memcpy(send_info.Iname, chat_info->Iname, 100);
    memcpy(send_info.send_name, chat_info->send_name, 100);
    for (int i = 1; i < MAX_SOCKS; ++i) {
        if (strcmp(UserNamelist[i], send_info.send_name) == 0) {
            send(pollFd[i].fd, (char*)&send_info, sizeof(CHAT_INFOR), 0);
            send(pollFd[i].fd, (char*)&file_data, sizeof(Data_info), 0);
            while ((nRead = fread(szBuffer, sizeof(char), 4096, outFile)) > 0) {
                // 发送数据
                send(pollFd[i].fd, szBuffer, nRead, 0);
            }
        }
    }
    TRACE("发送文件！\r\n");
    // 关闭文件
    fclose(outFile);

    return 0;
}

int DistributeMessages(char* data, SOCKET Sock, int i, Cnet_serverDlg* dlg)
{
    struct CHAT_INFOR* chat_info;
    char buffer[4096] = { 0 };
    int bytesRecv;
    CString set_t;
    char out_buf[DATA_MAX] = { 0 };

    chat_info = (struct CHAT_INFOR*)data;

    switch (chat_info->typ) {
    case 1:
        bytesRecv = recv(Sock, buffer, chat_info->len, 0);
        sprintf(out_buf, " %s to 群聊 ：%s\n", chat_info->Iname, buffer);
        set_t.Format(out_buf);
        dlg->chat_1.SetSel(-1);
        dlg->chat_1.ReplaceSel(set_t);
        SentToPublicRoom(buffer, chat_info);
        break;
    case 2:
        sprintf(out_buf, "用户 %s 加入\n", chat_info->Iname);
        set_t.Format(out_buf);
        dlg->chat_1.SetSel(-1);
        dlg->chat_1.ReplaceSel(set_t);

        sprintf(UserNamelist[i], "%s", chat_info->Iname);
        printf("name %s\r\n", UserNamelist[i]);
        SentUserInfo(dlg);
        break;
    case 3:
        bytesRecv = recv(Sock, buffer, chat_info->len, 0);
        sprintf(out_buf, " %s to %s ：%s\n", chat_info->Iname, chat_info->send_name, buffer);
        set_t.Format(out_buf);
        dlg->chat_1.SetSel(-1);
        dlg->chat_1.ReplaceSel(set_t);
        SentToSideRoom(buffer, chat_info);
        break;
    case 4:
        Gout_user(chat_info->send_name);
        SentUserInfo(dlg);
        break;
    case 5:
        bytesRecv = recv(Sock, buffer, sizeof(Data_info), 0);
        RecvFiletransfer(buffer, Sock);

        sprintf(out_buf, "用户 %s 发送文件到群聊\n", chat_info->Iname);
        set_t.Format(out_buf);
        dlg->chat_1.SetSel(-1);
        dlg->chat_1.ReplaceSel(set_t);
        SentFileTohome(buffer, chat_info);
        break;
    case 6:
        bytesRecv = recv(Sock, buffer, sizeof(Data_info), 0);
        RecvFiletransfer(buffer, Sock);

        sprintf(out_buf, "用户 %s 发送文件到 %s \n", chat_info->Iname, chat_info->send_name);
        set_t.Format(out_buf);
        dlg->chat_1.SetSel(-1);
        dlg->chat_1.ReplaceSel(set_t);
        SentFileToUser(buffer, chat_info);
        break;
    default:break;
    }

    return 0;
}

unsigned int __stdcall ListeningSocket(void* information)
{
    struct socket_infor* socket_t;
    socket_t = (struct socket_infor*)information;
    Cnet_serverDlg* dlg;
    // 定义 pollfd 数组
    int result;
    ZeroMemory(pollFd, sizeof(pollFd));
    char recv_info[sizeof(CHAT_INFOR)];

    dlg = socket_t->dlg;
    // 将监听 socket 加入 pollFd 数组
    pollFd[0].fd = socket_t->socket;
    pollFd[0].events = POLLIN;

    while (working_flag) {
        // 使用 WSAPoll 函数等待事件
        result = WSAPoll(pollFd, MAX_SOCKS, 5000);
        if (result == SOCKET_ERROR) {

            printf("WSAPoll failed with error: %d\n", WSAGetLastError());
            break;
        }
        else if (result == 0) {
            continue;
        }
        else {
            // 处理事件
            for (int i = 0; i < MAX_SOCKS; ++i) {
                if (pollFd[i].revents == 0) {
                    continue;
                }
                if (i == 0) {
                    // 监听 socket 上有连接请求
                    if (pollFd[i].revents & POLLIN) {
                        struct sockaddr_in clientAddr;
                        int addrLen = sizeof(clientAddr);
                        SOCKET clientSock = accept(socket_t->socket, (SOCKADDR*)&clientAddr, &addrLen);
                        if (clientSock == INVALID_SOCKET) {
                            printf("accept failed with error: %d\n", WSAGetLastError());
                        }
                        else {
                            printf("Accepted a new connection from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                            // 将新连接的 socket 加入 pollFd 数组
                            for (int j = 1; j < MAX_SOCKS; ++j) {
                                if (pollFd[j].fd == 0) {
                                    pollFd[j].fd = clientSock;
                                    pollFd[j].events = POLLIN;
                                    break;
                                }
                            }
                        }
                    }
                }
                else {
                    // 客户端 socket 上有数据到达
                    if (pollFd[i].revents & POLLIN) {
                        char buffer[1024] = { 0 };
                        int bytesRecv = recv(pollFd[i].fd, recv_info, sizeof(CHAT_INFOR), 0);
                        if (bytesRecv == SOCKET_ERROR) {
                            printf("recv failed with error: %d\n", WSAGetLastError());
                        }
                        else if (bytesRecv == 0) {
                            printf("Connection closed by the other side.\n");
                            closesocket(pollFd[i].fd);
                            pollFd[i].fd = 0;
                            pollFd[i].events = 0;
                        }
                        else {

                            printf("Received %d bytes from socket %d: %s\n", bytesRecv, pollFd[i].fd, buffer);
                            // 原样发送数据给客户端
                            DistributeMessages(recv_info, pollFd[i].fd, i, dlg);
                        }
                    }
                    // 客户端 socket 出现异常
                    if (pollFd[i].revents & (POLLERR | POLLHUP)) {
                        printf("An error occurred on socket %d.\n", pollFd[i].fd);
                        closesocket(pollFd[i].fd);
                        pollFd[i].fd = 0;
                        pollFd[i].events = 0;
                        SentUserInfo(dlg);
                    }
                }
            }
        }
    }

    // 关闭监听 socket
    closesocket(socket_t->socket);

    // 清理 Winsock 库
    WSACleanup();

    return 0;
}

int creat_chat_room(Cnet_serverDlg* dlg, const char *host, int port)
{
    SOCKET serverSocket;
    HANDLE childProcessHandle;
    unsigned int childThreadId;
    struct socket_infor* socket_t = new struct socket_infor;
    CString set_t;
    char out_buf[DATA_MAX] = { 0 };

    serverSocket = creat_socket(dlg, host, port);

    socket_t->socket = serverSocket;
    socket_t->dlg = dlg;
    // 创建子进程
    childProcessHandle = (HANDLE)_beginthreadex(NULL, 0, &ListeningSocket, (void*)socket_t, 0, &childThreadId);

    if (childProcessHandle == 0) {
        dlg->chat_1.SetWindowTextA(_T("Failed to create child process."));
        return 1;
    }

    sprintf(out_buf, "success to create child process!\n");
    set_t.Format(out_buf);
    dlg->chat_1.SetWindowTextA(set_t);

    return 0;
}

int close_socket()
{
    working_flag = 0;
    return 0;
}