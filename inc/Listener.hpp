#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "parser2.hpp"
#include <arpa/inet.h>
#include "Connection.hpp"
#include <vector>

class Server;
class Connection;

class Listener {
	public:
		int						port;
		int						sock;
		std::vector<Server>&	servers;
		struct sockaddr_in		sockaddr;

		Listener(int port, std::vector<Server>& servers);
		Listener(Listener const& other);
		~Listener();
		void listenMode() const;
		const Connection handleEvent(short revents) const;
	};

#endif
