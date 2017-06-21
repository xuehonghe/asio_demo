#include "connection_mgr.h"

#include "connect_socket.h"

extern connection_mgr* g_connMgr_ = nullptr;

connection_mgr::connection_mgr()
	: closeCallback_(nullptr)
{
	g_connMgr_ = this;
}

connection_mgr::~connection_mgr()
{
}

void connection_mgr::join(std::shared_ptr<connect_socket> sock)
{
	if (sock)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		socks_[sock->id()] = sock;
	}
}

void connection_mgr::leave(std::shared_ptr<connect_socket> sock)
{
	if (sock)
	{
		auto id = sock->id();
		auto& peerEp = sock->socket_.remote_endpoint();
		auto& addr = peerEp.address().to_string();
		unsigned short port = peerEp.port();
		
		if (closeCallback_)
		{
			closeCallback_(id, addr.c_str(),port);
		}
		std::lock_guard<std::mutex> lock(mutex_);
		socks_.erase(id);
	}
}

void connection_mgr::clear()
{
	std::lock_guard<std::mutex> lock(mutex_);

	for (auto& conn : socks_)
	{
		//connect_socket
		//conn.second->stop();
	}
	std::map<int, std::shared_ptr<connect_socket> > socks_;
}

void connection_mgr::send(const int sockid, const message_codec& msg)
{
	std::lock_guard<std::mutex> lock(mutex_);
	
	auto itr = socks_.find(sockid);
	if (itr != socks_.end())
	{
		auto& sock = itr->second;
		if (sock)
		{
			sock->send(msg);
		}
	}

}

void connection_mgr::setCloseCallback(CloseCallback closeCallback)
{
	closeCallback_ = closeCallback;
}
