#include "Server.h"

public ref class server_managed {
public:
	int Init(const char* address, int port) { return ser->Init(address, port); }

	//更新数据
	void Run() { ser->Run(); }
	void close() {
		ser->Run();
	}
private:
	CServerNet *ser;
};