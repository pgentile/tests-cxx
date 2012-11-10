//
//  Hello World client
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include <zmq.hpp>
#include <cstring>
#include <iostream>
#include <sstream>

using std::string;


int main (void)
{
	try {
		zmq::context_t context(1);

		//  Socket to talk to server
		std::cout << "Connecting to hello world server..." << std::endl;
		zmq::socket_t socket(context, ZMQ_REQ);
	
		const char* identity = "C++ZClient";
		socket.setsockopt(ZMQ_IDENTITY, (const void *) identity, strlen(identity));

		socket.connect("tcp://localhost:9000");

		int request_nbr;
		for (request_nbr = 0; request_nbr != 10; request_nbr++) {
			std::stringstream dataBuilder;
			dataBuilder << "Hello #" << request_nbr;
			
			string const data(dataBuilder.str());
			zmq::message_t request(data.size() * sizeof(char));
			std::memcpy(request.data(), data.c_str(), request.size());
			
			socket.send(request);
		
			zmq::message_t reply;
			socket.recv(&reply);
			std::cout << "Received World " << request_nbr << std::endl;
		}
	}
	catch (zmq::error_t const & err) {
		std::cout << "Erreur : " << err.what() << std::endl;
	}

	return 0;
}
