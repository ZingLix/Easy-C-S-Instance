#pragma once
#include <stdio.h>
#include <winsock.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>


//using namespace std;
#pragma comment (lib,"ws2_32.lib")

class CServerNet
{
public:

	//初始化服务器,返回0表示成功
	int Init(const char* address, int port);

	//更新数据
	void Run();
	void close() {
		closesocket(m_sock);
	}
	int clientCount() { return msglist.size(); }
	int clientMsgCount(int i) { return msglist[i]->msg.size(); }
	int clientID(int i) { return msglist[i]->id; }
	std::string* msg(int i,int j) { return msglist[i]->msg[j]; }
	int errMsgCount() { return errMsgList.size(); }
	std::string* errMsg(int i) { return errMsgList[i]; }
	void CServerNet::RevMsgThread(SOCKET newSocket);
private:
	struct clientInfo;
	std::vector<clientInfo*> msglist;
	SOCKET m_sock;
	std::vector<std::string*> errMsgList;
	std::vector<SOCKET> clientList;

	struct clientInfo {
		int id;
		std::vector<std::string*> msg;
		clientInfo(SOCKET S) { id = S; }
	};
};