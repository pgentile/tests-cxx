#ifndef IO_FILE_FILE_HPP
#define IO_FILE_FILE_HPP

#include <cstdio>
#include <cerrno>
#include <string>
#include <memory>
#include <unistd.h>


namespace io {
namespace file {

class File final
{

public:

    static size_t const DEFAULT_BUFFER_SIZE = BUFSIZ;

    static File open(char const* name, char const* mode, size_t bufferSize = DEFAULT_BUFFER_SIZE);

    File(File const&) = delete;

    File(File&& src);

    ~File();

    File& operator =(File const&) = delete;

    File& operator =(File&& src);

    void write(std::string const& s);

    void write(char const* s, size_t length);

    void write(char const* s);

    void write(char c);

    void flush();
    
    void sync();

    void truncate(off_t length);

    long int tell();

    void close();

    int getDescriptor();

    bool isOpen() const {
        return !_closed;
    }

    std::string const& getName() const {
        return _name;
    }

private:

    File(FILE* file, std::string&& name, size_t bufferSize);

    [[ noreturn ]] static void throwSystemError();

    FILE* _file;

    std::string _name;

    std::unique_ptr<char[]> _buffer;

    bool _closed;

};

} // namespace file
} // namespace io

#endif
