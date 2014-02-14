#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cassert>
#include <cerrno>
#include <system_error>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <boost/numeric/conversion/cast.hpp>

#include "patterns/ExceptionSafe.hpp"

using namespace std;
using boost::numeric_cast;


namespace memmapped {


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

void lock(const void* addr, size_t length) {
    int rc = mlock(addr, length);
    if (rc == -1) {
        throw system_error(errno, system_category());
    }
}

void unlock(const void* addr, size_t length) {
    int rc = munlock(addr, length);
    if (rc == -1) {
        throw system_error(errno, system_category());
    }
}

int getPageSize() {
    return getpagesize();
}


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
    
    MemMapped(FileDescriptor& descriptor, size_t length, int protections, int flags, off_t offset = 0):
        _length(length),
        _start(NULL)
    {
        int fd = descriptor.fd();
        _start = mmap(NULL, length, protections, flags | MAP_FILE, fd, offset);
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
    
    friend ostream& operator <<(ostream& out, MemMapped const& mapped);
    

private:
    
    MemMapped(MemMapped const &);
    
    MemMapped& operator =(MemMapped const &);
    
    size_t const _length;

    void* _start;

};


ostream& operator <<(ostream& out, MemMapped const& mapped) {
    out << "MemMapped(length = " << mapped._length
        << ", start = " << mapped._start << ")";
    return out;
}


class Lock {

public:
    
    Lock(void* addr, size_t length):
        _addr(addr),
        _length(length)
    {
        lock(addr, length);
    }
    
    ~Lock() {
        EXCEPTION_SAFE_BEGIN();
        unlock(_addr, _length);
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
    
    MemView(MemMapped & mapped, T* start, T* end):
        _mapped(mapped),
        _start(start),
        _end(end),
        _lock(start, numeric_cast<size_t>(end - start))
    {
    }
    
    ~MemView() {
    }
    
    void sync(int flags) {
        ::memmapped::sync(_start, numeric_cast<size_t>(_end - _start), flags);
    }
    
    T* start() const {
        return _start;
    }
    
    T* end() const {
        return _end;
    }
    
    ptrdiff_t length() const {
        return _end - _start;
    }
    
    template<typename TT>
    friend ostream& operator <<(ostream& out, MemView<TT> const& view);

private:
    
    MemView(MemView const &);
    
    MemView& operator =(MemView const &);
    
    MemMapped & _mapped;
    
    T* _start;
    
    T* _end;
    
    Lock _lock;
    
};

template<typename T>
ostream& operator <<(ostream& out, MemView<T> const& view) {
    out << "MemView(start = " << static_cast<void const *>(view._start)
        << ", length = " << view.length()
        << ", type size = " << sizeof(T) << ")";
    return out;
}


}


using namespace memmapped;


int main() {
    cout << "Test mmap" << endl;
    
    size_t count = 10;
    size_t length = count * sizeof(char);
    
    FileDescriptor descriptor("bite", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(descriptor.fd(), numeric_cast<off_t>(length));
    
    MemMapped mapped(descriptor, length, PROT_READ | PROT_WRITE, MAP_SHARED);
    MemView<char> view(
        mapped,
        mapped.addr<char>(0),
        mapped.addr<char>(numeric_cast<ptrdiff_t>(count))
    );
    
    cout << "About MemMapped: " << mapped << endl;
    cout << "About MemView: " << view << endl;
        
    return 0;
}
