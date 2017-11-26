#include "Client.h"

public ref class Client_managed {
public:
	Client_managed() { client = new CClientNet(); }
	int Connect(int port, const char* address) { return client->Connect(port, address); }
	int SendMsg(const char* msg, int len) { return client->SendMsg(msg, len); }
	void Close() { client->Close(); }

private:
	CClientNet *client;
};