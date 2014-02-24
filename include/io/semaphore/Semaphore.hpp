#ifndef IO_SEMAPHORE_SEMAPHORE_HPP
#define IO_SEMAPHORE_SEMAPHORE_HPP

#include <semaphore.h>


namespace io {
namespace semaphore {


class Semaphore
{
public:

    Semaphore();

    virtual ~Semaphore();

    int getValue();
    
    void post();
    
    void wait();

protected:
    
    virtual sem_t* pt() = 0;

    [[ noreturn ]] static void throwSystemError();

};


}
}

#endif
