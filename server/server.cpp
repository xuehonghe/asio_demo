#include "server.h"

#include "connect_socket.h"

#include "../message_codec.hpp"

server::server()
	: acceptor_(io_service_)
	, socket_(io_service_)
	, conn_id_(0)
	, connCallback_(nullptr)
	, thrdnum_(1)
	, start_status_(false)
{
}

server::~server()
{
	stop();
}

bool server::start(const boost::asio::ip::tcp::endpoint& endpoint, size_t thrdnum)
{
	if (start_status_ && endpoint_ == endpoint)
	{
		return true;
	}
	
	try
	{
		acceptor_.close();
		if (io_service_.stopped())
		{
			io_service_.reset();
		}
		acceptor_.open(endpoint.protocol());
		//acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

		acceptor_.bind(endpoint);
		acceptor_.listen();
		do_accept();
	}
	catch (std::exception& e)
	{
		std::string err = e.what();
		return false;
	}
	catch (...)
	{
		return false;
	}
	thrdnum_ = thrdnum_ > thrdnum ? thrdnum : thrdnum_;
	for (unsigned int i = 0; i < thrdnum_; ++i)
	{
		srv_thrds_.push_back(std::move(std::thread([this]() {io_service_.run(); })));
	}
	
	//io_srv_thrd_ = std::thread(std::bind(io_service_, &boost::asio::io_service::run));
	endpoint_ = endpoint;
	start_status_ = true;
	return true;
}

void server::stop()
{
	acceptor_.close();
	start_status_ = false;

	for (auto& thrd : srv_thrds_)
	{
		thrd.join();
	}
	srv_thrds_.clear();
	io_service_.stop(); //线程先退出还是 io_service 先 stop;???
	thrdnum_ = 0;
	conn_id_ = 0;
	//connmgr_.leave();
}

void server::setConnectCallback(ConnectCallback connCallback)
{
	connCallback_ = connCallback;
}


void server::sendMessage(const int session_id, char cmd_id, const char* msg, unsigned int msg_len)
{
	message_codec msgCodec;
	msgCodec.encode(cmd_id, msg, msg_len);
	connmgr_.send(session_id, msgCodec);
}

void server::setMessageCallback(MessageCallback msgCallback)
{
	connect_socket::setMessageCallback(msgCallback);
}

void server::setCloseCallback(CloseCallback closeCallback)
{
	connmgr_.setCloseCallback(closeCallback);
}


void server::do_accept()
{
	++conn_id_;
	acceptor_.async_accept(socket_,
		[this](boost::system::error_code ec)
	{
		if (!ec)
		{
			auto& peerEp = socket_.remote_endpoint();
			auto& addr = peerEp.address().to_string();
			unsigned short port = peerEp.port();
			if (connCallback_)
			{
				connCallback_(conn_id_, addr.c_str(), port);
			}
			
			auto connection = std::make_shared<connect_socket>(std::move(socket_), conn_id_, connmgr_);

			connection->start();

			do_accept();
		}
		else
		{
			std::string err= ec.message();
		}
	});
}

