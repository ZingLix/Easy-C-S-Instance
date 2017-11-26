//#include "Client.h"
//#include <string>
//void main()
//{
//	CClientNet client;
//
//	//连接到127.0.0.1（即本地）,端口号为8888的服务端
//	printf("Connect\n");
//	client.Connect(8888, "127.0.0.1");
//
//	//发送消息
//	printf("send msg\n");
//	std::string message;
//	while (1) {
//		std::cin >> message;
//		client.SendMsg(message.data(), message.size());
//	}
//	
//
//	//关闭socket
//	printf("close\n");
//	client.Close();
//
//	system("pause");
//}