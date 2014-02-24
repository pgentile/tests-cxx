#ifndef IO_SEMAPHORE_ANONYMOUS_SEMAPHORE_HPP
#define IO_SEMAPHORE_ANONYMOUS_SEMAPHORE_HPP

#include "io/semaphore/Semaphore.hpp"


namespace io {
namespace semaphore {


class AnonymousSemaphore: public Semaphore {

public:
    
    AnonymousSemaphore(unsigned int count, bool shared = false);
    
    AnonymousSemaphore(AnonymousSemaphore const&) = delete;
    
    AnonymousSemaphore(AnonymousSemaphore&& src);
    
    ~AnonymousSemaphore();
    
    AnonymousSemaphore& operator =(AnonymousSemaphore const&) = delete;
    
    AnonymousSemaphore& operator =(AnonymousSemaphore&& src);
    
    void destroy();
    
protected:
    
    virtual sem_t* pt() override;
    
private:
    
    sem_t _sem;

};


}
}

#endif
