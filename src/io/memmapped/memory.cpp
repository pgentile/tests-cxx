#include "io/memmapped/memory.hpp"

#include <system_error>
#include <cerrno>
#include <sys/mman.h>
#include <sys/types.h>

using namespace std;


namespace io {
namespace memmapped {
namespace memory {


void sync(void* addr, size_t length, int flags) {
    int rc = msync(addr, length, flags);
    if (rc == -1) {
        throw system_error(errno, system_category());
    }
}

void protect(void* addr, size_t length, int protections) {
    int rc = mprotect(addr, length, protections);
    if (rc == -1) {
        throw system_error(errno, system_category());
    }
}

void lock(void* addr, size_t length) {
    int rc = mlock(addr, length);
    if (rc == -1) {
        throw system_error(errno, system_category());
    }
}

void unlock(void* addr, size_t length) {
    int rc = munlock(addr, length);
    if (rc == -1) {
        throw system_error(errno, system_category());
    }
}


} // namespace memory
} // namespace memmapped
} // namespace io
