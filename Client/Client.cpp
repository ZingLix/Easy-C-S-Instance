#include "Client.h"
#include <thread>
int ClientClass::Connect(int port, const char* address)
{
	int rlt = 0;

	//���ڼ�¼������Ϣ�����
	int iErrMsg;
	//����WinSock
	WSAData wsaData;
	iErrMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (iErrMsg != NO_ERROR)
		//�д���
	{
		printf("failed with wsaStartup error : %d\n", iErrMsg);
		ErrMsgList.push_back(new std::string("WSA����ʧ�ܡ�ERR: " + std::to_string(iErrMsg)));
		rlt = 1;
		return rlt;
	}

	//����Socket
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)
		//����Socketʧ��
	{
		printf("socket failed with error : %d\n", WSAGetLastError());
		ErrMsgList.push_back(new std::string("SOCKET����ʧ�ܡ�ERR: " + std::to_string(iErrMsg)));
		rlt = 2;
		return rlt;
	}

	//Ŀ�����������
	sockaddr_in sockaddrServer;
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_port = port;
	sockaddrServer.sin_addr.s_addr = inet_addr(address);

	//����,sock��Ŀ�����������
	iErrMsg = connect(m_sock, (sockaddr*)&sockaddrServer, sizeof(sockaddrServer));
	if (iErrMsg < 0)
	{
		printf("connect failed with error : %d\n", iErrMsg);
		ErrMsgList.push_back(new std::string("����ʧ�ܡ�������δ������"));

		rlt = 3;
		return rlt;
	}
	ErrMsgList.push_back(new std::string("�ѽ������ӣ�"+GetIP()+":"+std::to_string(GetPort())));
	status = 1;
	infoCountCurrent = 0;
	std::thread thr(&ClientClass::RecvMsg, this);
	thr.detach();
	return rlt;
}




int ClientClass::SendMsg(const char* msg, int len)
{
	int rlt = 0;

	int iErrMsg = 0;

	//������Ϣ��ָ��sock������Ϣ
	iErrMsg = send(m_sock, msg, len, 0);
	if (iErrMsg < 0)
		//����ʧ��
	{
		printf("send msg failed with error : %d\n", iErrMsg);
		ErrMsgList.push_back(new std::string("send msg failed with error : " + std::to_string(iErrMsg)));

		rlt = 1;
		return rlt;
	}

	return rlt;
}

void ClientClass::RecvMsg() {
	char buf[1024];
	memset(buf, '\0', 1024);
		char CloseMsg[] = "CloseConnection";
		char ServerCloseMsg[] = "ServerClosed";
	do{
		int k = recv(m_sock, buf, 1024, 0);
		if (!strcmp(buf,CloseMsg)) {
			Close("�������ر�������");
			break;
		}
		else if (!strcmp(buf, ServerCloseMsg)) {
			Close("�������ر�");
		}
		if (status == 0) break;
		ErrMsgList.push_back(new std::string(buf));
		infoCountAll++;
		infoCountCurrent++;
		memset(buf, '\0', 1024);
	} while (1);
}

void ClientClass::Close(std::string str)
{
	status = 0;
	ErrMsgList.push_back(new std::string(str));
	closesocket(m_sock);
}

std::string ClientClass::GetIP() {
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	getpeername(m_sock, (SOCKADDR *)&addr_conn, &nSize);
	std::string s = inet_ntoa(addr_conn.sin_addr);
	return s;
}

int ClientClass::GetPort() {
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	getpeername(m_sock, (SOCKADDR *)&addr_conn, &nSize);
	int p = addr_conn.sin_port;
	return p;
}

int ClientClass::GetStatus() {
	return status;
}