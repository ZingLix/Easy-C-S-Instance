#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<windows.h>

#pragma comment(lib, "Ws2_32.lib")

class ClientClass
{
public:
	ClientClass() :infoCountAll(0) {}
	int Connect(int port, const char* address);
	int SendMsg(const char* msg, int len);
	int errMsgCount() { return ErrMsgList.size(); }
	std::string* errMsg(int i) { return ErrMsgList[i]; }
	void RecvMsg();
	void Close(std::string str = "客户端关闭了连接");
	std::string GetIP();
	int GetPort();
	int GetStatus();
	int GetAllInfoCount() { return infoCountAll; }
	int GetCurrentInfoCount() { return infoCountCurrent; }
	~ClientClass() { Close(); }
private:
	int infoCountCurrent;
	int infoCountAll;
	SOCKET m_sock;
	int status;
	std::vector<std::string*> ErrMsgList;
};