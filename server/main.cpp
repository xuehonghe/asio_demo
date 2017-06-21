
#include "../server_api.h"

#include <string>
#include <iostream>

void onMessageCallback(const int session_id, char cmd_id, const char* msg, unsigned int msg_len)
{
	std::cout << "onMessage: session id:" << session_id << ",cmd id:" << (int)cmd_id << ",message:";
	std::cout.write(msg, msg_len);
	std::cout << std::endl;
}

void onConnectCallback(const int session_id, const char* ip, unsigned short port)
{
	std::cout << "onConnect addr:" << ip << ",port:" << port << std::endl;
}

void onCloseCallback(const int session_id, const char* ip, unsigned short port)
{
	std::cout << "onClose addr:" << ip << ",port:" << port << std::endl;
}

int main(int argc, char* argv[])
{
	const char* ip = "127.0.0.1";
	unsigned short port = 9500;

	setMessageCallback(onMessageCallback);
	setConnectCallback(onConnectCallback);
	setCloseCallback(onCloseCallback);
	if (startServer(ip, port))
	{
		stopServer();
		//startServer(ip, port);
	}
	//short port1 = 9501;
	//startServer(ip, port1);
	startServer(ip, port);
	
	while (getchar() != 'q')
	{
	}

	return 0;
}
