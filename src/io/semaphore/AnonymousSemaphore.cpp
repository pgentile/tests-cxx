#include "io/semaphore/AnonymousSemaphore.hpp"


namespace io {
namespace semaphore {


AnonymousSemaphore::AnonymousSemaphore(unsigned int count, bool shared) {
    if (sem_init(&_sem, static_cast<int>(shared), count) != 0) {
        throwSystemError();
    }
}

AnonymousSemaphore::AnonymousSemaphore(AnonymousSemaphore&& src):
    _sem(src._sem)
{
}

AnonymousSemaphore::~AnonymousSemaphore() {
    
}

AnonymousSemaphore& AnonymousSemaphore::operator =(AnonymousSemaphore&& src) {
    _sem = src._sem;
    return *this;
}

void AnonymousSemaphore::destroy() {
    if (sem_destroy(&_sem) != 0) {
        throwSystemError();
    }
}

sem_t* AnonymousSemaphore::pt() {
    return &_sem;
}

}
}
