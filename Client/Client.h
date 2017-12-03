#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<windows.h>

#pragma comment(lib, "Ws2_32.lib")

class CClientNet
{
public:
	//连接上指定服务器
	int Connect(int port, const char* address);
	//发送信息
	int SendMsg(const char* msg, int len);
	//关闭
	int errMsgCount() { return ErrMsgList.size(); }
	std::string* errMsg(int i) { return ErrMsgList[i]; }
	void RecvMsg();
	void Close(std::string str="客户端关闭了连接");
	std::string GetIP();
	int GetPort();
	int GetStatus();

private:
	SOCKET m_sock;
	int status;
	std::vector<std::string*> ErrMsgList;
};