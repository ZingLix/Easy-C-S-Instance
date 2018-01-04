#include "Server.h"

#include <thread>
int ServerClass::Init(const char* address, int port)
{
	int rlt = 0;
	int iErrorMsg;

	WSAData wsaData;

	iErrorMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);

	if (iErrorMsg != NO_ERROR)
	{
		errMsgList.push_back(new std::string("wsastartup failed with error : " + std::to_string(iErrorMsg)));
		rlt = 1;
		return rlt;
	}

	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)

	{
		errMsgList.push_back(new std::string("socket failed with error : " + std::to_string(iErrorMsg)));
		rlt = 2;
		return rlt;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = inet_addr(address);

	iErrorMsg = bind(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

	if (iErrorMsg < 0)
	{
		errMsgList.push_back(new std::string("bind failed with error : " + std::to_string(iErrorMsg)));

		rlt = 3;
		return rlt;
	}

	return rlt;
}

void ServerClass::Run()
{
	listen(m_sock, 5);
	ServerStatus = 1;
	sockaddr_in tcpAddr;
	int len = sizeof(sockaddr);
	SOCKET newSocket;

	do
	{
		newSocket = accept(m_sock, (sockaddr*)&tcpAddr, &len);


		if (newSocket == INVALID_SOCKET)
		{

		}
		else
		{
			std::thread thr(&ServerClass::RevMsgThread, this, newSocket);
			thr.detach();
		}



	} while (1);

	closesocket(m_sock);
}

void  ServerClass::RevMsgThread(SOCKET newSocket) {

	char buf[1024];
	memset(buf, '\0', 1024);
	int rval;
	clientList.push_back(newSocket);
	msglist.push_back(new clientInfo(newSocket));
	int count = msglist.size() - 1;
	do
	{
		printf("process\n");
		memset(buf, '\0', 1024);
		rval = recv(newSocket, buf, 1024, 0);


		if (rval == SOCKET_ERROR) {
			printf("recv socket error\n");
			errMsgList.push_back(new std::string("recv socket error"));
			break;
		}

		if (rval == 0)

		{
			printf("ending connection");
			break;
		}
		else {


			msglist[count]->msg.push_back(new std::string(buf));
		}
		memset(buf, '\0', 1024);

	} while (rval != 0 && ServerStatus == 1);
	clientList.erase(std::find(clientList.begin(), clientList.end(), newSocket));
	closesocket(newSocket);
}

void ServerClass::SendMsg(int index, std::string s) {
	char buf[1024];
	int i;
	for (i = 0; i < 1024; i++) {
		buf[i] = s.c_str()[i];
	}

	SOCKET S = msglist[index]->Soc;
	send(S, buf, strlen(buf), 0);
}

void ServerClass::SendMsg(int index, char* s, int len) {
	SOCKET S = msglist[index]->Soc;
	send(S, s, len, 0);
}

void ServerClass::CloseClient(int index, int flag) {
	if (flag != 0) SendMsg(index, "ServerClosed");
	else SendMsg(index, "CloseConnection");
	msglist[index]->status = 1;
}

int ServerClass::GetStatus(int index) {
	return msglist[index]->status;
}

std::string ServerClass::GetIP(int index)
{
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	getpeername(msglist[index]->Soc, (SOCKADDR *)&addr_conn, &nSize);
	std::string s = inet_ntoa(addr_conn.sin_addr);
	return s;
}

int ServerClass::GetPort(int index)
{
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	getpeername(msglist[index]->Soc, (SOCKADDR *)&addr_conn, &nSize);
	int s = addr_conn.sin_port;
	return s;
}

void ServerClass::close() {
	for (int i = 0; i < msglist.size(); i++) {
		if (msglist[i]->status == 0) CloseClient(i, 1);
	}
	closesocket(m_sock);
	ServerStatus = 0;
}

ServerClass::~ServerClass() {
	close();
}