#pragma once
#include <stdio.h>
#include <winsock.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;
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
	int msgCount() { return msglist.size(); }
	string* msg(int i) { return msglist[i]; }
	int errMsgCount() { return errMsgList.size(); }
	string* errMsg(int i) { return errMsgList[i]; }

private:
	vector<string*> msglist;
	SOCKET m_sock;
	vector<string*> errMsgList;
};