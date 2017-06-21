#define SOCKET_SERVER_API extern  "C"  __declspec(dllexport)

#include "../server_api.h"
#include "server.h"


server g_srv;

SOCKET_SERVER_API bool startServer(const char* ip, unsigned short port, size_t thrdnum)
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
	
	return g_srv.start(endpoint, thrdnum);
}

SOCKET_SERVER_API void stopServer()
{
	g_srv.stop();
}

SOCKET_SERVER_API void sendMessage(const int session_id, char cmd_id, const char* msg, unsigned int msg_len)
{
	g_srv.sendMessage(session_id, cmd_id, msg, msg_len);
}

SOCKET_SERVER_API void setMessageCallback(MessageCallback msgCallback)
{
	g_srv.setMessageCallback(msgCallback);
}

SOCKET_SERVER_API void setConnectCallback(ConnectCallback connCallback)
{
	g_srv.setConnectCallback(connCallback);
}

SOCKET_SERVER_API void setCloseCallback(CloseCallback closeCallback)
{
	g_srv.setCloseCallback(closeCallback);
}
