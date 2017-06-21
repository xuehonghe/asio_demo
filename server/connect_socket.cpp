#include "connect_socket.h"

#include "connection_mgr.h"

#include <iostream>

using boost::asio::ip::tcp;

connect_socket::connect_socket(tcp::socket socket, unsigned int id, connection_mgr& room)
	: socket_(std::move(socket))
	, connmgr_(room)
	, id_(id)
{
}

connect_socket::~connect_socket()
{
}

void connect_socket::start()
{
	connmgr_.join(shared_from_this());
	do_read_header();
}

void connect_socket::send(const message_codec& msg)
{
	bool write_in_progress = !write_msgs_.empty();
	write_msgs_.push_back(msg);
	if (!write_in_progress)
	{
		do_write();
	}
}


void connect_socket::setMessageCallback(MessageCallback msgCallback)
{
	msgCallback_ = msgCallback;
}

void connect_socket::do_read_header()
{
	auto self(shared_from_this());
	boost::asio::async_read(socket_,
		boost::asio::buffer(read_msg_.begin(), message_codec::header_length),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec && read_msg_.decode_header())
		{
			do_read_body();
		}
		else
		{
			connmgr_.leave(shared_from_this());
		}
	});
}

void connect_socket::do_read_body()
{
	auto self(shared_from_this());
	boost::asio::async_read(socket_, 
		boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			char cmd_id;
			unsigned short data_len;
			const char* data = nullptr;
			read_msg_.decode_body(cmd_id, &data, data_len);

			if (msgCallback_)
			{
				msgCallback_(id_, cmd_id, data, data_len);
			}
			//connmgr_.deliver(read_msg_);
			do_read_header();
		}
		else
		{
			connmgr_.leave(shared_from_this());
		}
	});
}

void connect_socket::do_write()
{
	auto self(shared_from_this());
	boost::asio::async_write(socket_,
		boost::asio::buffer(write_msgs_.front().begin(),write_msgs_.front().length()),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			write_msgs_.pop_front();
			if (!write_msgs_.empty())
			{
				do_write();
			}
		}
		else
		{
			connmgr_.leave(shared_from_this());
		}
	});
}

MessageCallback connect_socket::msgCallback_ = nullptr;
