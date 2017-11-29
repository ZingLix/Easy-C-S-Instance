#include "Client.h"

public ref class Client_managed {
public:
	Client_managed() { client = new CClientNet(); }
	int Connect(int port, const char* address) { return client->Connect(port, address); }
	int SendMsg(const char* msg, int len) { return client->SendMsg(msg, len); }
	void Close() { client->Close(); }
	int errMsgCount() { return client->errMsgCount(); }
	System::String^ errMsg(int i) {
		std::string str = *client->errMsg(i);
		System::String ^S = gcnew System::String(str.c_str());
		return S;
	}
private:
	CClientNet *client;
};