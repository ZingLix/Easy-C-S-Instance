#include "Server.h"

#include <thread>
int CServerNet::Init(const char* address, int port)
{
	int rlt = 0;

	//用于记录错误信息，并输出
	int iErrorMsg;

	//初始化WinSock
	WSAData wsaData;
	iErrorMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);

	if (iErrorMsg != NO_ERROR)
	{
		//初始化WinSock失败
		errMsgList.push_back(new std::string("wsastartup failed with error : " + std::to_string(iErrorMsg)));
		rlt = 1;
		return rlt;
	}

	//创建服务端Socket
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)

	{
		//创建Socket异常
		errMsgList.push_back(new std::string("socket failed with error : " + std::to_string(iErrorMsg)));

		rlt = 2;
		return rlt;
	}

	//设置套接字非阻塞模式  
	//unsigned long ul = 1;
	//rlt = ioctlsocket(m_sock, FIONBIO, (unsigned long*)&ul);
	//if (SOCKET_ERROR == rlt)
	//	return FALSE;

	//声明信息
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = inet_addr(address);

	//绑定
	iErrorMsg = bind(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (iErrorMsg < 0)
	{
		//绑定失败
		errMsgList.push_back(new std::string("bind failed with error : " + std::to_string(iErrorMsg)));

		rlt = 3;
		return rlt;
	}

	return rlt;
}

void CServerNet::Run()
{
	//公开连接
	listen(m_sock, 5);
	ServerStatus = 1;
	sockaddr_in tcpAddr;
	int len = sizeof(sockaddr);
	SOCKET newSocket;

	do
	{
		//接收信息
		newSocket = accept(m_sock, (sockaddr*)&tcpAddr, &len);


		if (newSocket == INVALID_SOCKET)
		{
			//非可用socket
			//errMsgList.push_back(new std::string("asdfa"));
			//Sleep(100);
			//int nErrCode = WSAGetLastError();
			//if (nErrCode == WSAEWOULDBLOCK)  //无法立即完成一个非阻挡性套接字操作  
			//{
			//	Sleep(100);
			//	continue;//继续等待  
			//}
			//else
			//{
			//	break;//线程退出  
			//}
		}
		else
		{
			//新socket连接
		//	char *pClientIP = inet_ntoa(tcpAddr.sin_addr);
		//	u_short  clientPort = ntohs(tcpAddr.sin_port);
			printf("new socket connect : %d\n", newSocket);
			//	errMsgList.push_back(new std::string("new socket connect : "+ std::to_string(newSocket)+",IP:"+pClientIP+",port:"+std::to_string(clientPort)));

				//消息处理
			//	using namespace System::Threading;
			//	Thread ^ oThread = gcnew Thread(gcnew ParameterizedThreadStart(&CServerNet::RevMsgThread));
			std::thread thr(&CServerNet::RevMsgThread, this, newSocket);
//			ClientThreadPool.push_back(thr);
			thr.detach();

		}



	} while (1);

	//关闭自身的Socket
	closesocket(m_sock);
}

void  CServerNet::RevMsgThread(SOCKET newSocket){

	char buf[1024];
	int rval;
	clientList.push_back(newSocket);
	msglist.push_back(new clientInfo(newSocket));
	int count = msglist.size()-1;
	do
	{
		printf("process\n");
		//接收数据
		memset(buf, 0, sizeof(buf));
		rval = recv(newSocket, buf, 1024, 0);


		if (rval == SOCKET_ERROR) {
			//这应该是个异常，当客户端没有调用closeSocket就直接退出游戏的时候，将会进入这里
			printf("recv socket error\n");
			errMsgList.push_back(new std::string("recv socket error"));
			break;
		}

		if (rval == 0)
			//recv返回0表示正常退出
		{
			printf("ending connection");
	//		msglist[count]->status = 1;
			break;
		}
		else {

			//显示接收到的数据
			msglist[count]->msg.push_back(new std::string(buf));
		}


	} while (rval != 0);
	clientList.erase(std::find(clientList.begin(),clientList.end(),newSocket));
	closesocket(newSocket);
}

void CServerNet::SendMsg(int index,std::string s) {
	char buf[1024];
	//memset(buf, '\0',1024);
	//buf[s.length()] = '\0';
	int i;
	for ( i = 0; i < s.length(); i++) {
		buf[i] = s[i];
	}
	buf[i] = '\0';
	SOCKET S = msglist[index]->Soc;
	send(S, buf, strlen(buf), 0);
}

void CServerNet::CloseClient(int index,int flag) {
	if (flag != 0) SendMsg(index, "ServerClosed");
	else SendMsg(index, "CloseConnection");
	msglist[index]->status = 1;
}

int CServerNet::GetStatus(int index) {
	return msglist[index]->status;
}

std::string CServerNet::GetIP(int index)
{
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	getpeername(msglist[index]->Soc, (SOCKADDR *)&addr_conn, &nSize);
	std::string s = inet_ntoa(addr_conn.sin_addr);
	return s;
}

int CServerNet::GetPort(int index)
{
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	getpeername(msglist[index]->Soc, (SOCKADDR *)&addr_conn, &nSize);
	int s = addr_conn.sin_port;
	return s;
}

void CServerNet::close() {
	for (int i = 0; i < msglist.size(); i++) {
		if (msglist[i]->status == 0) CloseClient(i,1);
	}
	closesocket(m_sock);
	ServerStatus = 0;
}