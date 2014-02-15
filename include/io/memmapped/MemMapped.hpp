#ifndef IO_MEMMAPPED_MEM_MAPPED_HPP
#define IO_MEMMAPPED_MEM_MAPPED_HPP

#include <iostream>
#include <system_error>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <boost/numeric/conversion/cast.hpp>

#include "io/memmapped/Memory.hpp"
#include "util/ExceptionSafe.hpp"


namespace io {
namespace memmapped {
    
    using namespace std;
    using boost::numeric_cast;
    
    class FileDescriptor
    {

    public:

        FileDescriptor(string const & filename, int flags):
            _fd(open(filename.c_str(), flags))
        {
            if (_fd == -1) {
                throw system_error(errno, system_category());
            }
        }

        FileDescriptor(string const & filename, int flags, int mode):
            _fd(open(filename.c_str(), flags, mode))
        {
            if (_fd == -1) {
                throw system_error(errno, system_category());
            }
        }

        ~FileDescriptor() {
            EXCEPTION_SAFE_BEGIN();
            if (_fd != -1 && close(_fd) == -1) {
                throw system_error(errno, system_category());
            }
            EXCEPTION_SAFE_END();
        }

        int fd() const {
            return _fd;
        }

    private:

        FileDescriptor(FileDescriptor const &);

        FileDescriptor& operator =(FileDescriptor const &);

        int _fd;
    };
    
    
    class MemMapped {

    public:

        MemMapped(size_t length, int protections, int flags, int fd, off_t offset = 0):
            _length(length),
            _start(NULL)
        {
            _start = mmap(NULL, length, protections, flags, fd, offset);
            if (_start == MAP_FAILED) {
                throw system_error(errno, system_category());
            }
        }

        ~MemMapped() {
            EXCEPTION_SAFE_BEGIN();
            if (munmap(_start, _length) == -1) {
                throw system_error(errno, system_category());
            }
            EXCEPTION_SAFE_END();
        }

        void* start() const {
            return _start;
        }

        void* end() const {
            return static_cast<char*>(_start) + _length;
        }

        template<typename T>
        T* addr(ptrdiff_t offset) const {
            return static_cast<char*>(_start) + offset;
        }

        size_t length() const {
            return _length;
        }
        
        void* sync(ptrdiff_t offset, size_t length, int flags) {
            void* addr = this->addr<char>(offset);
            sync(addr, length, flags);
            return addr;
        }

        void* protect(ptrdiff_t offset, size_t length, int protections) {
            void* addr = this->addr<char>(offset);
            protect(addr, length, protections);
            return addr;
        }

        void* lock(ptrdiff_t offset, size_t length) {
            void* addr = this->addr<char>(offset);
            lock(addr, length);
            return addr;
        }

        void* unlock(ptrdiff_t offset, size_t length) {
            void* addr = this->addr<char>(offset);
            unlock(addr, length);
            return addr;
        }
        
        static void sync(void* addr, size_t length, int flags) {
            int rc = msync(addr, length, flags);
            if (rc == -1) {
                throw system_error(errno, system_category());
            }
        }

        static void protect(void* addr, size_t length, int protections) {
            int rc = mprotect(addr, length, protections);
            if (rc == -1) {
                throw system_error(errno, system_category());
            }
        }

        static void lock(void* addr, size_t length) {
            int rc = mlock(addr, length);
            if (rc == -1) {
                throw system_error(errno, system_category());
            }
        }

        static void unlock(void* addr, size_t length) {
            int rc = munlock(addr, length);
            if (rc == -1) {
                throw system_error(errno, system_category());
            }
        }

        static int getPageSize() {
            return getpagesize();
        }

        friend ostream& operator <<(ostream& out, MemMapped const& mapped);

    private:

        MemMapped(MemMapped const &);

        MemMapped& operator =(MemMapped const &);

        size_t const _length;

        void* _start;

    };


    std::ostream& operator <<(std::ostream& out, MemMapped const& mapped) {
        out << "MemMapped(length = " << mapped._length
            << ", start = " << mapped._start << ")";
        return out;
    }
    
    
    class Lock {

    public:

        Lock(MemMapped& mapped, ptrdiff_t offset, size_t length):
            _addr(mapped.lock(offset, length)),
            _length(length)
        {
        }

        ~Lock() {
            EXCEPTION_SAFE_BEGIN();
            MemMapped::unlock(_addr, _length);
            EXCEPTION_SAFE_END();
        }

    private:

        Lock(Lock const &);

        Lock& operator =(Lock const &);
        
        void* _addr;

        size_t const _length;

    };


    template<typename T>
    class MemView
    {

    public:

        MemView(MemMapped & mapped, ptrdiff_t offset, size_t length):
            _mapped(mapped),
            _start(mapped.addr<T>(offset)),
            _length(length)
        {
        }

        ~MemView() {
        }

        void sync(int flags) {
            MemMapped::sync(_start, _length * sizeof(T), flags);
        }

        T* start() const {
            return _start;
        }

        T* end() const {
            return _start + _length;
        }

        size_t length() const {
            return _length;
        }

        template<typename TT>
        friend ostream& operator <<(ostream& out, MemView<TT> const& view);

    private:

        MemView(MemView const &);

        MemView& operator =(MemView const &);

        MemMapped & _mapped;

        T* _start;

        size_t _length;

    };


    template<typename T>
    std::ostream& operator <<(std::ostream& out, MemView<T> const& view) {
        out << "MemView(start = " << static_cast<void const *>(view._start)
            << ", length = " << view.length()
            << ")";
        return out;
    }

} // namespace memmapped
} // namespace io

#endif
