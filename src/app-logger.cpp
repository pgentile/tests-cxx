#include <iostream>
#include <string>

#include "logger/Logger.hpp"

using namespace logger;


int main(void) {
    LoggerManager loggerManager;
    loggerManager.log(Level::info, "Log me");
    
    return 0;
}
