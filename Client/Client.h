#pragma once
#include <iostream>
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
	void Close();

private:
	SOCKET m_sock;
};