#include "webserv.hpp"
#include <sys/poll.h>

Connection::Connection(int port, int sock, const std::vector<Server>& servers): port(port), sock(sock), servers(servers), req(NULL), res(NULL), data("") {

	if (fcntl(sock, F_SETFD, O_CLOEXEC) == -1)
		throw std::runtime_error("fcntl error: " + std::string(strerror(errno)));
	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl error: " + std::string(strerror(errno)));
	startTimer();
}

//if smt around here fails, check this, bc its not currently a full copy
Connection::Connection(Connection const& other): startTime(other.startTime), checkTime(other.checkTime), port(other.port), sock(other.sock), servers(other.servers), req(other.req), data(other.data) {}

Connection::~Connection() {}

void Connection::startTimer() {
	startTime = time(NULL);
	checkTime = true;
}

// returns true if timeout
// 60s timeout
bool Connection::checkTimer() const {
	return (checkTime && startTime + 60 <= time(NULL));
}

//a connection is uniquely identified by sock fd
bool Connection::operator==(const Connection& other) const {
	return (other.sock == sock);
}

//0 means ok, 1 means remove
int Connection::handleEvent(struct pollfd& pollfd) {
	if (pollfd.revents & POLLERR) {
		std::cout << "pollerr" << std::endl;
		return 1;
	}
	else if (pollfd.revents & POLLHUP) {
		std::cout << "pollhup" << std::endl;
		return 1;
	}
	else if (pollfd.revents & POLLIN) {
		std::cout << "pollin" << std::endl;

		int read_res;
		char *read_buff = new char[SIZE_READ];

		memset(read_buff, 0, SIZE_READ);
		read_res = recv(sock, read_buff, SIZE_READ, 0);
		std::cout << read_buff << std::endl;

		if (read_res <= 0) {
			delete[] read_buff;
			return 1;
		}

		data += read_buff;
		delete[] read_buff;
	}

	//response handling should be done here, not in pollin
	//si hay un error en la request (4xx, 5xx) devolvemos Connection: close y cerramos conexion
	else if (pollfd.revents & POLLOUT) {
		std::cout << "pollout" << std::endl;
		
		std::string httpResponse = res->getHttpResponse();
		
		//probably have to handle send return value
		std::string s = "HTTP/1.1 200 OK\r\n\
Content-Length: 0\r\n\
Connection: keep-alive\r\n\
Content-Type: text/plain; charset=utf-8\r\n\
\r\n";

		if (send(sock, s.c_str(), 103, 0) <= 0)
			return 1;
		//call Response constructor, we pass request

		pollfd.events = POLLIN;
		//after send
		startTimer();
		delete req;
		req = NULL;
	}

	if (data != "") {
		if (!req)
			req = new Request();

		data = req->addData(data);
		if (req->parsed == Request::ALL) {
			checkTime = false;
			if(res == NULL){ // AQUI NO LLEGA EL VECTOR DE SERVIDORES Y NO ENCUENTRO DONDE SE LOCALIZA LA CONSTRUCCION DE LA CONNECTION PARA VER QUE OCURRE
				std::cout << *req << std::endl;
				res = new Response(toString(port), getServerByHost(servers, req->headers.at("Host")), *req);
			}
			
			int fdRet = 0;
			//TODO CHANGE THIS CONDITION BACK TO fdRet == 0
			while(fdRet != -1)
				fdRet = res->prepareResponse();
			
			if(fdRet == -1){
				pollfd.events = POLLOUT;
				delete res;
			}

			//ELSE INSERT FDRET AL POLL COMPLEJO, POR QUE CUANDO LEEMOS EL CONTENIDO DE UN ARCHIVO ES DISTINTO QUE CUANDO PROCESAMOS UN CGI
		}
	}

	return 0;
}