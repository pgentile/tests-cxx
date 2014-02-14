#include <zmq.hpp>

#include <unistd.h>
#include <string>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>


using std::string;


void receiveStr(zmq::socket_t& socket, string& s, int flags = 0) {
	zmq::message_t request;
	if (!socket.recv(&request, flags)) {
		std::cerr << "Reception KO" << std::endl;
	}
	
	s.clear();
	s.append((char const *) request.data(), request.size());
}


void sendStr(zmq::socket_t& socket, string const & s, int flags = 0) {
	zmq::message_t reply(s.size() * sizeof(string::value_type));
	std::memcpy(reply.data(), s.c_str(), s.size());
	
	if (!socket.send(reply, flags)) {
		std::cerr << "Envoi KO" << std::endl;
	}
}


void sendEmpty(zmq::socket_t& socket, int flags = 0) {
	zmq::message_t reply(0);	
	if (!socket.send(reply, flags)) {
		std::cerr << "Envoi KO" << std::endl;
	}
}


namespace {
	volatile std::sig_atomic_t interrupted = 0;
}


void handleInterrupt(int signal) {
	interrupted = 1;
	std::cout << "Interrupt handled for signal " << signal << std::endl;
}


int main(int argc, char const * const * argv)
{
	// std::signal(SIGINT, handleInterrupt);
	
	try {
		if (argc < 2) {
			std::cerr << "Pas assez d'argument" << std::endl;
			std::exit(1);
		}
		
		char const * url = argv[1];
		
		zmq::context_t context(1);
	
		std::cout << "Binding to " << url << std::endl;
		zmq::socket_t socket(context, ZMQ_DEALER);
		socket.bind(url);
		
		unsigned int i = 0;
		while (true) {
			std::cout << "Waiting..." << std::endl;
			sleep(1);
			
			// Empty delimited, required for communication with a REP socket
			sendEmpty(socket, ZMQ_SNDMORE);
			
			std::stringstream request;
			request << "My request #" << i;
			sendStr(socket, request.str());
			std::cout << "Request sent: " << request.str() << std::endl;
			
			//string response;
			//receiveStr(socket, response);
			//std::cout << "Response received: " << response << std::endl;
			
			i++;
		}
	}
	catch (zmq::error_t const & err) {
		std::cout << "ZMQ Error: " << err.what() << std::endl;
	}

	return 0;
}
