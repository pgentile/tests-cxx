#ifndef IO_MEMMAPPED_MEMORY_HPP
#define IO_MEMMAPPED_MEMORY_HPP

#include <cstddef>
#include <unistd.h>


namespace io {
namespace memmapped {
namespace memory {


void sync(void* addr, size_t length, int flags);

void protect(void* addr, size_t length, int protections);

void lock(void* addr, size_t length);

void unlock(void* addr, size_t length);

inline int getPageSize() {
    return getpagesize();
}

}
} // namespace memmapped
} // namespace io

#endif
