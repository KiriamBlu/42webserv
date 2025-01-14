#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "Connection.hpp"
#include "parser.hpp"
#include <arpa/inet.h>
#include <vector>

class Server;
class Connection;

class Listener {
	public:
		int port;
		int sock;
		const std::vector<Server> &servers;
		struct sockaddr_in sockaddr;

		Listener(unsigned port, const std::vector<Server> &servers);
		Listener(Listener const &other);
		~Listener();
		void listenMode() const;
		const Connection handleEvent(short revents) const;
};

#endif
