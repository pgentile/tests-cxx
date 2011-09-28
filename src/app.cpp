#include <iostream>

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
	
	cout << "Demarrage du programme" << endl;

	LoggerManager loggerManager;
	
	loggerManager.log("Test d'un log 1");
	loggerManager.log("Test d'un log 2");
	
	unsigned int duration = 10;
	cout << "On dort..." << endl;
	sleep(duration);

	cout << "Arret du programme" << endl;
	
	return rc;
}
