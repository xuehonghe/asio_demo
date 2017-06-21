#ifndef  _SOCKE_SERVER_H_
#define  _SOCKE_SERVER_H_

#ifndef  SOCKET_SERVER_API
#define SOCKET_SERVER_API extern  "C"  __declspec(dllimport)
#endif

typedef void(*MessageCallback)(const int session_id, char cmd_id, const char* msg, unsigned int msg_len);
typedef void(*ConnectCallback)(const int session_id, const char* ip, unsigned short port);
typedef void(*CloseCallback)(const int session_id, const char* ip, unsigned short port);

SOCKET_SERVER_API void setMessageCallback2(MessageCallback msgCallback);

SOCKET_SERVER_API void setMessageCallback(MessageCallback msgCallback);
SOCKET_SERVER_API void setConnectCallback(ConnectCallback connCallback);
SOCKET_SERVER_API void setCloseCallback(CloseCallback closeCallback);

SOCKET_SERVER_API bool startServer(const char* ip, unsigned short port, size_t thrdnum = 1);
SOCKET_SERVER_API void stopServer();
SOCKET_SERVER_API void sendMessage(const int session_id, char cmd_id, const char* msg, unsigned int msg_len);



#endif // _SOCKE_SERVER_H_