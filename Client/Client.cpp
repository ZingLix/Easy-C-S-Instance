#include "Client.h"
#include <thread>
int CClientNet::Connect(int port, const char* address)
{
	int rlt = 0;

	//用于记录错误信息并输出
	int iErrMsg;
	//启动WinSock
	WSAData wsaData;
	iErrMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (iErrMsg != NO_ERROR)
		//有错误
	{
		printf("failed with wsaStartup error : %d\n", iErrMsg);
		ErrMsgList.push_back(new std::string("failed with wsaStartup error : " + std::to_string(iErrMsg)));
		rlt = 1;
		return rlt;
	}

	//创建Socket
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)
		//创建Socket失败
	{
		printf("socket failed with error : %d\n", WSAGetLastError());
		ErrMsgList.push_back(new std::string("socket failed with error : " + std::to_string(iErrMsg)));
		rlt = 2;
		return rlt;
	}

	//目标服务器数据
	sockaddr_in sockaddrServer;
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_port = port;
	sockaddrServer.sin_addr.s_addr = inet_addr(address);

	//连接,sock与目标服务器连接
	iErrMsg = connect(m_sock, (sockaddr*)&sockaddrServer, sizeof(sockaddrServer));
	if (iErrMsg < 0)
	{
		printf("connect failed with error : %d\n", iErrMsg);
		ErrMsgList.push_back(new std::string("connect failed with error : " + std::to_string(iErrMsg)));

		rlt = 3;
		return rlt;
	}
	ErrMsgList.push_back(new std::string("ok: " + std::to_string(iErrMsg)));
	status = 1;
	std::thread thr(&CClientNet::RecvMsg, this);
	thr.detach();
	return rlt;
}




int CClientNet::SendMsg(const char* msg, int len)
{
	int rlt = 0;

	int iErrMsg = 0;

	//发送消息，指定sock发送消息
	iErrMsg = send(m_sock, msg, len, 0);
	if (iErrMsg < 0)
		//发送失败
	{
		printf("send msg failed with error : %d\n", iErrMsg);
		ErrMsgList.push_back(new std::string("send msg failed with error : " + std::to_string(iErrMsg)));

		rlt = 1;
		return rlt;
	}

	return rlt;
}

void CClientNet::RecvMsg() {
	char buf[1024];
	memset(buf, '\0', 1024);
		char CloseMsg[] = "CloseConnection";
	do{
		int k = recv(m_sock, buf, 1024, 0);
		if (!strcmp(buf,CloseMsg)) {
			Close();
			break;
		}
		if (status == 0) break;
		ErrMsgList.push_back(new std::string(buf));
		memset(buf, '\0', 1024);
	} while (1);
}

void CClientNet::Close()
{
	status = 0;
	closesocket(m_sock);
}

std::string CClientNet::GetIP() {
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	getpeername(m_sock, (SOCKADDR *)&addr_conn, &nSize);
	std::string s = inet_ntoa(addr_conn.sin_addr);
	return s;
}

int CClientNet::GetPort() {
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	getpeername(m_sock, (SOCKADDR *)&addr_conn, &nSize);
	int p = addr_conn.sin_port;
	return p;
}

int CClientNet::GetStatus() {
	return status;
}