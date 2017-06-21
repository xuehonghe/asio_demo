#pragma once
#include <memory>
#include <deque>
#include <boost/asio.hpp>

#include "../message_codec.hpp"
#include "../server_api.h"

class message_codec;
class connection_mgr;

class connect_socket : public std::enable_shared_from_this<connect_socket>
{
public:
	connect_socket(boost::asio::ip::tcp::socket socket, unsigned int id, connection_mgr& room);
	~connect_socket();

public:
	void start();
	void send(const message_codec& msg);
	unsigned int id() { return id_; }

public:
	static void setMessageCallback(MessageCallback msgCallback);

private:
	void do_read_header();
	void do_read_body();

	void do_write();

public:
	boost::asio::ip::tcp::socket socket_;
private:
	unsigned int id_;
	connection_mgr& connmgr_;
	message_codec read_msg_;
	std::deque<message_codec> write_msgs_;

	static MessageCallback msgCallback_;
};

