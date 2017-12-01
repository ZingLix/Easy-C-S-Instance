#include "Server.h"

public ref class server_managed {
public:
	server_managed() { ser = new CServerNet(); }
	int Init(const char* address, int port) { return ser->Init(address, port); }

	//更新数据
	void Run() { ser->Run(); }
	void close() {
		ser->Run();
	}
	int clientCount() { return ser->clientCount(); }
	int clientMsgCount(int i) { return ser->clientMsgCount(i); }
	int clientID(int i) { return ser->clientID(i); }
	System::String^ msg(int i,int j) { 
		std::string str = *ser->msg(i,j);
		System::String ^S = gcnew System::String(str.c_str());
		return S;
	}

	int errMsgCount() { return ser->errMsgCount(); }
	System::String^ errMsg(int i) {
		std::string str = *ser->errMsg(i);
		System::String ^S = gcnew System::String(str.c_str());
		return S;
	}

private:
	CServerNet *ser;
};