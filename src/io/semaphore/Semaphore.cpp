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
