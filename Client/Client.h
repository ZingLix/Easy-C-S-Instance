#pragma once
#include <iostream>
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
	void Close();

private:
	SOCKET m_sock;
};