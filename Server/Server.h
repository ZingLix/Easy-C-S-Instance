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

class ServerClass
{
public:

	int Init(const char* address, int port);

	void Run();
	void close();
	int clientCount() { return msglist.size(); }
	int clientMsgCount(int i) { return msglist[i]->msg.size(); }
	int clientID(int i) { return msglist[i]->id; }
	std::string* msg(int i, int j) { return msglist[i]->msg[j]; }
	int errMsgCount() { return errMsgList.size(); }
	std::string* errMsg(int i) { return errMsgList[i]; }
	void RevMsgThread(SOCKET newSocket);
	void SendMsg(int index, std::string s);
	void SendMsg(int index, char* s, int len);
	void CloseClient(int index, int flag = 0);
	int GetStatus(int index);
	std::string GetIP(int index);
	int GetPort(int index);
	int GetServerStatus() { return ServerStatus; }
	int GetClientInfoCount(int i) { return msglist[i]->msg.size(); }
	~ServerClass();

private:
	struct clientInfo;
	std::vector<clientInfo*> msglist;
	SOCKET m_sock;
	std::vector<std::string*> errMsgList;
	std::vector<SOCKET> clientList;
	int ServerStatus;
	//std::vector<std::thread&> ClientThreadPool;

	struct clientInfo {
		int id;
		SOCKET Soc;
		std::vector<std::string*> msg;
		int status;
		clientInfo(SOCKET S) { id = S; Soc = S; status = 0; }
	};
};