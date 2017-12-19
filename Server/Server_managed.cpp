#include "Server.h"
#include <msclr\marshal_cppstd.h>

public ref class server_managed {
public:
	server_managed() { ser = new ServerClass(); }
	int Init(const char* address, int port) { return ser->Init(address, port); }

	//更新数据
	void Run() { ser->Run(); }
	void close() {
		ser->close();
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

	void SendMsg(int index, System::String^ s) {
		std::string str= msclr::interop::marshal_as<std::string>(s);
		ser->SendMsg(index, str);
	}

	void SendMsg(int index, char *msg,int len) {
		ser->SendMsg(index, msg,len);
	}

	void CloseClient(int index) {
		ser->CloseClient(index);
	}

	int GetStatus(int index) { return ser->GetStatus(index); }

	System::String^ GetIP(int index) {
		std::string str = ser->GetIP(index);
		System::String ^S = gcnew System::String(str.c_str());
		return S;
	}
	int GetPort(int index) { return ser->GetPort(index); }
	int GetServerStatus() { return ser->GetServerStatus(); }
	int GetClientInfoCount(int i) { return ser->GetClientInfoCount(i); }
private:
	ServerClass *ser;
};