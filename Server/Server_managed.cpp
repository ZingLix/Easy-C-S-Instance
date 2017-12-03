#include "Server.h"
#include <msclr\marshal_cppstd.h>

public ref class server_managed {
public:
	server_managed() { ser = new CServerNet(); }
	int Init(const char* address, int port) { return ser->Init(address, port); }

	//��������
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

	void SendMsg(int index, System::String^ s) {
		std::string str= msclr::interop::marshal_as<std::string>(s);
		ser->SendMsg(index, str);
	}

	void CloseClient(int index) {
		ser->CloseClient(index);
	}
private:
	CServerNet *ser;
};