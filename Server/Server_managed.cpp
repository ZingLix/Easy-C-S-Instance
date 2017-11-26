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
	int msgCount() { return ser->msgCount(); }
	System::String^ msg(int i) { 
		string str = *ser->msg(i);
		System::String ^S = gcnew System::String(str.c_str());
		return S;
	}
private:
	CServerNet *ser;
};