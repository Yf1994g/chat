#include "Chatroom_dlg.h"
#include "Myfrd_dlg.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

struct CHAT_INFOR {
	int typ;
	int len;
	char Iname[100];
	char send_name[100];
	char filename[100];
};

struct DLG_INFO{
	char name[100];
	Chatroom_dlg *Wroom;
};

struct Data_info {
	int typ;
	int len;
	char name[100];
	ULONGLONG file_size;
};

int Link_net(const char* host, int port);

int SetC_name(const char *data);

int RecverData(Myfrd_dlg *dlg);

int SentData(const char *data, Chatroom_dlg * dlg);

int CreatChatDlg(char *data, Myfrd_dlg *dlg);

int Gout_user(const char *name);

int ReleaseP(Chatroom_dlg * dlg);

int SentFileToServer(const char* file_path, Chatroom_dlg* dlg);

class CMyThread : public CWinThread {
public:

	Myfrd_dlg *dlg;
    virtual BOOL InitInstance() override {
        // 线程初始化
		while(true) {
			RecverData(dlg);
			Sleep(1000);
		}
        return TRUE;
    }

    virtual int ExitInstance() override {
        // 线程结束
		return CWinThread::ExitInstance();
    }
};