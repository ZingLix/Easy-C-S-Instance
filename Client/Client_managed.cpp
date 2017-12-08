#include "Client.h"

public ref class Client_managed {
public:
	Client_managed() { client = new ClientClass(); }
	int Connect(int port, const char* address) { return client->Connect(port, address); }
	int SendMsg(const char* msg, int len) { return client->SendMsg(msg, len); }
	void Close() { client->Close(); }
	int errMsgCount() { return client->errMsgCount(); }
	System::String^ errMsg(int i) {
		std::string str = *client->errMsg(i);
		System::String ^S = gcnew System::String(str.c_str());
		return S;
	}

	System::String^ GetIP() {
		std::string str = client->GetIP();
		System::String ^S = gcnew System::String(str.c_str());
		return S;
	}
	int GetPort() { return client->GetPort(); }
	int GetStatus() { return client->GetStatus(); }
	int GetAllInfoCount() { return client->GetAllInfoCount(); }
	int GetCurrentInfoCount() { return client->GetCurrentInfoCount(); }
private:
	ClientClass *client;
};