#include "Server.h"

class server_managed {
public:
	int Init(const char* address, int port) { ser.Init(address, port); }

	//��������
	void Run() { ser.Run(); }
	void close() {
		ser.Run();
	}
private:
	CServerNet ser;
};