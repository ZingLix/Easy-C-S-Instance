#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<windows.h>

#pragma comment(lib, "Ws2_32.lib")

class ClientClass
{
public:
	ClientClass():infoCountAll(0){}
	//������ָ��������
	int Connect(int port, const char* address);
	//������Ϣ
	int SendMsg(const char* msg, int len);
	//�ر�
	int errMsgCount() { return ErrMsgList.size(); }
	std::string* errMsg(int i) { return ErrMsgList[i]; }
	void RecvMsg();
	void Close(std::string str="�ͻ��˹ر�������");
	std::string GetIP();
	int GetPort();
	int GetStatus();
	int GetAllInfoCount() { return infoCountAll; }
	int GetCurrentInfoCount() { return infoCountCurrent; }

private:
	int infoCountCurrent;
	int infoCountAll;
	SOCKET m_sock;
	int status;
	std::vector<std::string*> ErrMsgList;
};