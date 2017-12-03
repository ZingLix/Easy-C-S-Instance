#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<windows.h>

#pragma comment(lib, "Ws2_32.lib")

class CClientNet
{
public:
	//������ָ��������
	int Connect(int port, const char* address);
	//������Ϣ
	int SendMsg(const char* msg, int len);
	//�ر�
	int errMsgCount() { return ErrMsgList.size(); }
	std::string* errMsg(int i) { return ErrMsgList[i]; }
	void RecvMsg();
	void Close();

private:
	SOCKET m_sock;
	std::vector<std::string*> ErrMsgList;
};