#include <iostream>
#include <sstream>

extern "C"
{
#include <unistd.h>
}

#include "logger.hpp"

using namespace std;
using namespace logger;
using namespace threading;

int main(void)
{
	int rc = 0;
	
	//cout << "Demarrage du programme" << endl;
	
	EventQueue queue(5);
	
	queue.publish(new LogEvent(Level::info, "Test"));
	queue.publish(new LogEvent(Level::info, "Test"));
	queue.publish(new LogEvent(Level::info, "Test"));
	queue.publish(new LogEvent(Level::info, "Test"));
	queue.publish(new LogEvent(Level::info, "Test"));
	queue.publish(new LogEvent(Level::info, "Test"));
	queue.publish(new LogEvent(Level::info, "Test"));
	queue.publish(new ShutdownEvent());
	queue.publish(new ShutdownEvent());

	/*
	LoggerManager loggerManager(Level::all, 100);
	
	for (int i = 0; i < 1000; i++) {
		ostringstream message;
		message << "Test d'un log " << i;
		loggerManager.log(Level::info, message.str());
	}
	*/
	
	//unsigned int duration = 5;
	//cout << "On dort..." << endl;
	//sleep(duration);
	
	//loggerManager.log(Level::warn, "On va bientot s'arreter");
	
	//cout << "On dort..." << endl;
	//sleep(duration);

	//cout << "Arret du programme" << endl;
	
	return rc;
}
