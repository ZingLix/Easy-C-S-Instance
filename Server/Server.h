#pragma once
#include <stdio.h>
#include <winsock.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
#pragma comment (lib,"ws2_32.lib")

class CServerNet
{
public:

	//��ʼ��������,����0��ʾ�ɹ�
	int Init(const char* address, int port);

	//��������
	void Run();
	void close() {
		closesocket(m_sock);
	}
	int msgCount() { return msglist.size(); }
	string* msg(int i) { return msglist[i]; }
	vector<string*> msglist;
private:
	SOCKET m_sock;
};