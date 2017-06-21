//
// Client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "../message_codec.hpp"

using boost::asio::ip::tcp;

typedef std::deque<message_codec> message_codec_queue;

class Client
{
public:
	Client(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator)
		: io_service_(io_service)
		, socket_(io_service)
	{
		do_connect(endpoint_iterator);
	}

	void write(const message_codec& msg)
	{
		io_service_.post(
		[this, msg]()
		{
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if (!write_in_progress)
			{
				do_write();
			}
		});
	}

	void close()
	{
		io_service_.post([this]() { socket_.close(); });
	}

private:
	void do_connect(tcp::resolver::iterator endpoint_iterator)
	{
		boost::asio::async_connect(socket_, endpoint_iterator, 
			[this](boost::system::error_code ec, tcp::resolver::iterator)
		{
			if (!ec)
			{
				do_read_header();
			}
		});
	}

	void do_read_header()
	{
		boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.begin(), message_codec::header_length),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec && read_msg_.decode_header())
			{
				do_read_body();
			}
			else
			{
				socket_.close();
			}
		});
	}

	void do_read_body()
	{
		boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				char cmd_id;
				unsigned short data_len;
				const char* data = nullptr;
				read_msg_.decode_body(cmd_id, &data, data_len);

				std::cout << "onMessage: cmd id:" << (int)cmd_id << ",message:";
				std::cout.write(data, data_len);
				std::cout << "\n";
				do_read_header();
			}
			else
			{
				socket_.close();
			}
		});
	}

	void do_write()
	{
		boost::asio::async_write(socket_,boost::asio::buffer(write_msgs_.front().begin(), write_msgs_.front().length()),
			[this](boost::system::error_code ec, std::size_t /*length*/)
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
				socket_.close();
			}
		});
	}

private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	message_codec read_msg_;
	message_codec_queue write_msgs_;
};

int main(int argc, char* argv[])
{
	std::string ip = "127.0.0.1";
	std::string port = "9500";
	try
	{
		/*
		char buf[64] = { 0 };
		std::cout << "please input connect address:";
		std::cin >> buf;
		ip = buf;
		std::cout << "please input connect port:";
		std::cin >> buf;
		port = buf;
		*/


		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		auto endpoint_iterator = resolver.resolve({ ip, port });
		Client clt(io_service, endpoint_iterator);

		std::thread t([&io_service]() { io_service.run(); });

		while (true)
		{
			std::cout << "请输入请求功能号：" << std::endl;
			char cmd_id[5] = { 0 };
			std::cin.getline(cmd_id, 5);
			char cmd_val = atoi(cmd_id);
			std::cout << "请输入序列号：" << std::endl;
			char line[message_codec::max_body_length + 1] = {0};
			std::cin.getline(line, message_codec::max_body_length + 1);
			
			std::cout << std::endl;

			message_codec msg;
			msg.encode(cmd_val, line, 16);

			//send
			clt.write(msg);
		}
		

		clt.close();

		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
