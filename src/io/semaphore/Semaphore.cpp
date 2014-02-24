#include "io/semaphore/Semaphore.hpp"

#include <system_error>
#include <semaphore.h>

using namespace std;


namespace io {
namespace semaphore {


Semaphore::Semaphore() {
}

Semaphore::~Semaphore() {
    
}

int Semaphore::getValue() {
    int value = 0;
    if (sem_getvalue(pt(), &value) != 0) {
        throwSystemError();
    }
    return value;
}

void Semaphore::post() {
    if (sem_post(pt()) != 0) {
        throwSystemError();
    }
}

void Semaphore::wait() {
    if (sem_wait(pt()) != 0) {
        throwSystemError();
    }
}

void Semaphore::throwSystemError() {
    throw system_error(errno, system_category());
}


}
}
