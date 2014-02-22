#include "io/file/File.hpp"

#include <system_error>

#include "util/ScopeGuard.hpp"
#include "util/exceptionsafe.hpp"

using namespace std;
using namespace util;


namespace io {
namespace file {


File File::open(char const* name, char const* mode, size_t bufferSize) {
    FILE* f = fopen(name, mode);
    if (f == nullptr) {
        throwSystemError();
    }
    ScopeGuard fileGuard = MAKE_SIMPLE_SCOPE_GUARD(fclose(f));
    
    File file(f, name, bufferSize);
    fileGuard.dismiss();
    return file;
}

File::File(FILE* file, string&& name, size_t bufferSize):
    _file(file),
    _name(move(name)),
    _buffer(),
    _closed(false)
{
    if (bufferSize == 0) {
        setvbuf(_file, nullptr, _IONBF, 0);
    }
    else if (bufferSize != DEFAULT_BUFFER_SIZE) {
        _buffer.reset(new char[bufferSize]);
        if (setvbuf(_file, _buffer.get(), _IOFBF, bufferSize) != 0) {
            throw runtime_error("Can't set buffer size");
        }
    }
}

File::File(File&& src):
    _file(src._file),
    _name(move(src._name)),
    _buffer(move(src._buffer)),
    _closed(src._closed)
{
    src._file = nullptr;
    src._closed = true;
}

File::~File() {
    EXCEPTION_SAFE_BEGIN();
    if (!_closed) {
        close();
    }
    EXCEPTION_SAFE_END();
}

File& File::operator =(File&& src) {
    _file = src._file;
    _name = move(src._name);
    _buffer = move(src._buffer);
    _closed = src._closed;
    
    src._file = nullptr;
    src._closed = true;
    
    return *this;
}

void File::write(string const& s) {
    write(s.c_str(), s.size());
}

void File::write(char const* s, size_t length) {
    if (fwrite(s, sizeof(char), length, _file) < length) {
        throwSystemError();
    }
}

void File::write(char const* s) {
    if (fputs(s, _file) == EOF) {
        throwSystemError();
    }
}

void File::write(char c) {
    if (fputc(c, _file) == EOF) {
        throwSystemError();
    }
}

void File::flush() {
    if (fflush(_file) == EOF) {
        throwSystemError();
    }
}

void File::sync() {
    flush();
    
    int fd = getDescriptor();
    if (fsync(fd) != 0) {
        throwSystemError();
    }
}

void File::truncate(off_t length) {
    flush();

    int fd = getDescriptor();
    if (ftruncate(fd, length) != 0) {
        throwSystemError();
    }
}

long int File::tell() {
    long int position = ftell(_file);
    if (position < 0) {
        throwSystemError();
    }
    return position;
}

void File::close() {
    _closed = true;
    if (fclose(_file) != 0) {
        throwSystemError();
    }
}

int File::getDescriptor() {
    if (_closed) {
        throw runtime_error("File is closed");
    }
    return fileno(_file);
}

// FIXME Passer le code d'erreur pour bien être sûr que c'est celui qui correspond à l'action
void File::throwSystemError() {
    throw system_error(errno, system_category());
}


}
}
