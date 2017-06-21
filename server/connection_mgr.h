#pragma once

#include <map>
#include <memory>
#include <mutex>

#include "../server_api.h"

class connect_socket;
class message_codec;

class connection_mgr
{
public:
	connection_mgr();
	~connection_mgr();

public:
	void join(std::shared_ptr<connect_socket> sock);

	void leave(std::shared_ptr<connect_socket> sock);
	void clear();

	void send(const int sockid, const message_codec& msg);

public:
	void setCloseCallback(CloseCallback closeCallback);


private:
	std::mutex mutex_; //shared_mutex
	std::map<int, std::shared_ptr<connect_socket> > socks_;

	CloseCallback closeCallback_;
};

