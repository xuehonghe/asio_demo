#pragma once

#include <boost/asio.hpp>

#include <vector>

#include "connection_mgr.h"
#include "../server_api.h"

class server
{
public:
	server();
	~server();

public:
	bool start(const boost::asio::ip::tcp::endpoint& endpoint, size_t thrdnum = 1);
	void stop();

public:
	void sendMessage(const int session_id, char cmd_id, const char* msg, unsigned int msg_len);
	void setMessageCallback(MessageCallback msgCallback);
	void setConnectCallback(ConnectCallback connCallback);
	void setCloseCallback(CloseCallback closeCallback);

private:
	void do_accept();
	

private:
	boost::asio::io_service io_service_;
	std::vector<std::thread> srv_thrds_;
	size_t thrdnum_;
	size_t conn_id_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::tcp::socket socket_;
	connection_mgr connmgr_;

	ConnectCallback connCallback_;

private: //
	boost::asio::ip::tcp::endpoint endpoint_;
	bool start_status_;
};

