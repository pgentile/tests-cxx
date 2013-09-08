#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <assert.h>
#include <cerrno>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "patterns/ExceptionSafe.hpp"


using namespace std;


class SystemException: public runtime_error {
    
public:
    
    SystemException(char const* source, int errnum):
        runtime_error(buildMessage(source, errnum))
    {
    }
    
    virtual ~SystemException() throw () {
    }
    
    int errnum() const {
        return _errnum;
    }

private:
    
    string buildMessage(char const* source, int errnum) {
        stringstream msg;
        msg << "System error with " << source << ": "
            << strerror(errnum) << " (num = " << errnum << ")";
        return msg.str();
    }
    
    int _errnum;
    
};


namespace memmapped {


void sync(void* addr, size_t length, int flags) {
    int rc = msync(addr, length, flags);
    if (rc == -1) {
        throw SystemException("msync", errno);
    }
}

void protect(void* addr, size_t length, int protections) {
    int rc = mprotect(addr, length, protections);
    if (rc == -1) {
        throw SystemException("mprotect", errno);
    }
}

void lock(const void* addr, size_t length) {
    int rc = mlock(addr, length);
    if (rc == -1) {
        throw SystemException("mlock", errno);
    }
}

void unlock(const void* addr, size_t length) {
    int rc = munlock(addr, length);
    if (rc == -1) {
        throw SystemException("munlock", errno);
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
            throw SystemException("open", errno);
        }
    }
    
    FileDescriptor(string const & filename, int flags, int mode):
        _fd(open(filename.c_str(), flags, mode))
    {
        if (_fd == -1) {
            throw SystemException("open", errno);
        }
    }
    
    ~FileDescriptor() {
        EXCEPTION_SAFE_BEGIN();
        if (close(_fd) == -1) {
            throw SystemException("close", errno);
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
            throw SystemException("mmap", errno);
        }
    }
    
    MemMapped(FileDescriptor& descriptor, size_t length, int protections, int flags, off_t offset = 0):
        _length(length),
        _start(NULL)
    {
        int fd = descriptor.fd();
        _start = mmap(NULL, length, protections, flags | MAP_FILE, fd, offset);
        if (_start == MAP_FAILED) {
            throw SystemException("mmap", errno);
        }
    }
    
    ~MemMapped() {
        EXCEPTION_SAFE_BEGIN();
        if (munmap(_start, _length) == -1) {
            throw SystemException("munmap", errno);
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
        _lock(start, end - start)
    {
    }
    
    ~MemView() {
    }
    
    void sync(int flags) {
        ::memmapped::sync(_start, _end - _start, flags);
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


int main(int argc, char const* argv[]) {
    cout << "Test mmap" << endl;
    
    size_t count = 10;
    size_t length = count * sizeof(char);
    
    FileDescriptor descriptor("bite", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(descriptor.fd(), length);
    
    MemMapped mapped(descriptor, length, PROT_READ | PROT_WRITE, MAP_SHARED);
    MemView<char> view(mapped, mapped.addr<char>(0), mapped.addr<char>(count));
    
    cout << "About MemMapped: " << mapped << endl;
    cout << "About MemView: " << view << endl;
        
    return 0;
}
