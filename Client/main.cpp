//#include "Client.h"
//#include <string>
//void main()
//{
//	CClientNet client;
//
//	//���ӵ�127.0.0.1�������أ�,�˿ں�Ϊ8888�ķ����
//	printf("Connect\n");
//	client.Connect(8888, "127.0.0.1");
//
//	//������Ϣ
//	printf("send msg\n");
//	std::string message;
//	while (1) {
//		std::cin >> message;
//		client.SendMsg(message.data(), message.size());
//	}
//	
//
//	//�ر�socket
//	printf("close\n");
//	client.Close();
//
//	system("pause");
//}