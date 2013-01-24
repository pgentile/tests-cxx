#include <zmq.hpp>

#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>

using std::string;


void receiveStr(zmq::socket_t& socket, string& s, int flags = 0) {
	zmq::message_t request;
	if (!socket.recv(&request, flags)) {
		std::cerr << "Reception KO" << std::endl;
	}
	
	s.append((char const *) request.data(), request.size());
}


void receiveAllStr(zmq::socket_t& socket, string& s, int flags = 0) {
	int64_t more = 1;
	size_t moreSize = sizeof(int64_t);
	while (more == 1) {
		receiveStr(socket, s, flags);
		socket.getsockopt(ZMQ_RCVMORE, &more, &moreSize);
	}
}


void sendStr(zmq::socket_t& socket, string const & s, int flags = 0) {
	zmq::message_t reply(s.size() * sizeof(string::value_type));
	std::memcpy(reply.data(), s.c_str(), s.size());
	
	if (!socket.send(reply, flags)) {
		std::cerr << "Envoi KO" << std::endl;
	}
}


int main(int argc, char const * const * argv)
{
	try {
		if (argc < 2) {
			std::cerr << "Pas assez d'argument" << std::endl;
			std::exit(1);
		}
		
		char const * url = argv[1];
		
		zmq::context_t context(1);
	
		std::cout << "Opening connection to " << url << std::endl;
		zmq::socket_t socket(context, ZMQ_REP);
		socket.connect(url);
		
		while (true) {
			std::cout << "Receiving..." << std::endl;
			string request;
			receiveAllStr(socket, request);
			std::cout << "Request received: " << request << std::endl;
			
			string response("Response for: ");
			response.append(request);
			std::cout << "Response sent: " << response << std::endl;
			sendStr(socket, response);

			std::cout << "Waiting..." << std::endl;
		}
	}
	catch (zmq::error_t const & err) {
		std::cout << "ZMQ Error: " << err.what() << std::endl;
	}

	return 0;
}
