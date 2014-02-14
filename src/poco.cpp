#include <iostream>
#include <sstream>
#include <cstdint>

#include "Poco/ThreadPool.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/HelpFormatter.h"

using namespace std;

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;


class RequestHandler: public HTTPRequestHandler {

public:
	
	RequestHandler(): HTTPRequestHandler() {
	}
	
	virtual ~RequestHandler() {
	}
	
	virtual void handleRequest(HTTPServerRequest& /*request*/, HTTPServerResponse& response) {
		response.setStatusAndReason(HTTPResponse::HTTP_OK, "OK");
		response.setContentType("text/plain");
		
		string buffer("Reponse OK");
		response.sendBuffer(buffer.c_str(), buffer.length());
	}
	
};


class RequestHandlerFactory: public HTTPRequestHandlerFactory {

public:
	
	RequestHandlerFactory(): HTTPRequestHandlerFactory() {
	}
	
	virtual ~RequestHandlerFactory() {
	}

	virtual HTTPRequestHandler* createRequestHandler(HTTPServerRequest const& /*request*/) {
		return new RequestHandler();
	}
	
};


class TestPocoApp: public Application {
	
public:
	
	TestPocoApp():
		Application(),
		_displayHelp(false),
		_port(0)
	{
		
	}
	
protected:
	
	virtual void defineOptions(OptionSet& options) {
		Option helpOpt("help", "h", "Aide");
		helpOpt.callback(OptionCallback<TestPocoApp>(this, &TestPocoApp::handleHelp));
		options.addOption(helpOpt);
		
		Option portOpt("port", "p", "Port d'écoute");
		portOpt
			.required(true)
			.argument("port")
			.callback(OptionCallback<TestPocoApp>(this, &TestPocoApp::handlePort));
		options.addOption(portOpt);
	}
		
	virtual int main(vector<string> const& /*args*/) {
		if (!_displayHelp) {
			cout << "Test POCO, port = " << _port << endl;
			
			HTTPRequestHandlerFactory::Ptr reqHandlerFactory(new RequestHandlerFactory());
			
			ServerSocket serverSocket(_port);
			
			HTTPServerParams::Ptr params(new HTTPServerParams());
			params->setMaxQueued(100); 
			params->setMaxThreads(16);
			
			HTTPServer server(reqHandlerFactory, serverSocket, params);
			server.start();
			
			cin.get();
			
			server.stop();
			ThreadPool::defaultPool().stopAll();
		}
		return EXIT_OK;
	}

private:	
	
	void handleHelp(string const& /*name*/, string const& /*value*/) {
		_displayHelp = true;
		displayHelp();
		stopOptionsProcessing();
	}
	
	void handlePort(string const& /*name*/, string const& value) {
		istringstream valueParser(value);
		valueParser >> _port;
	}
	
	void displayHelp() {
		/*
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setHeader("Petit test");
		helpFormatter.format(std::cout);
		*/
	}
	
	bool _displayHelp;
	
	uint16_t _port;

};

POCO_APP_MAIN(TestPocoApp);
