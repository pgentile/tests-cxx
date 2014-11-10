#include "io/memmapped/MemMapped.hpp"

#include <cassert>
#include <iostream>
#include <system_error>
#include <type_traits>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "io/memmapped/memory.hpp"
#include "util/exceptionsafe.hpp"

using namespace std;


namespace io {
namespace memmapped {

    MemMapped::MemMapped(size_t length, int protections, int flags, int fd, off_t offset):
        _start(nullptr),
        _end(nullptr),
        _length(length)
    {
        _start = mmap(nullptr, length, protections, flags, fd, offset);
        if (_start == MAP_FAILED) {
            throw system_error(errno, system_category());
        }
        _end = reinterpret_cast<char*>(_start) + length;
    }
            
    MemMapped::MemMapped(MemMapped&& src):
        _start(src._start),
        _end(src._end),
        _length(src._length)
    {    
        src._start = nullptr;
        src._end = nullptr;
    }

    MemMapped::~MemMapped() noexcept {
        EXCEPTION_SAFE_BEGIN();
        if (_start != nullptr) {
            if (munmap(_start, _length) == -1) {
                throw system_error(errno, system_category());
            }
        }
        EXCEPTION_SAFE_END();
    }
    
    MemMapped& MemMapped::operator =(MemMapped&& src) {
        _start = src._start;
        _end = src._end;
        _length = src._length;
        
        src._start = nullptr;
        src._end = nullptr;
        
        return *this;
    }

} // namespace memmapped
} // namespace io



namespace std {
    
ostream& operator <<(ostream& out, io::memmapped::MemMapped const& mapped) {
    out << "MemMapped(length = " << mapped.length()
        << ", start = " << mapped.start()
        << ", end  " << mapped.end() << ")";
    return out;
}

} // namespace std
