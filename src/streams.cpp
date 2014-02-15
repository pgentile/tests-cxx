#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <system_error>
#include <utility>
#include <clocale>
#include <cstdio>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "util/Optional.hpp"
#include "util/ScopeGuard.hpp"
#include "util/ExceptionSafe.hpp"

using std::cout;
using std::endl;


#define LOG(MSG) (cout << __func__ << ", L" << __LINE__ << ": " << MSG << endl)

#define OBJ_LOG(MSG) LOG("@ " << this << ": " << MSG)


class FileStreamBuffer: public std::filebuf
{

public:
    
    FileStreamBuffer(): std::filebuf() {
    }
    
    virtual ~FileStreamBuffer() {
    }

    std::filebuf* open(const char* filename, std::ios_base::openmode mode) {
        LOG("Ouverture de " << filename);
        std::filebuf* rc = std::filebuf::open(filename, mode);
        if (rc != NULL) {
            _filename = filename;
        }
        return rc;
    }

    std::filebuf* close() {
        LOG("Fermeture de " << *_filename);
        _filename.reset();
        return std::filebuf::close();
    }
    
    util::Optional<std::string> const& getFilename() const {
        return _filename;
    }

protected:
    
    virtual int_type underflow() {
        LOG("Appel");
        return std::filebuf::underflow();
    }
    
    virtual int_type overflow(int_type c = traits_type::eof()) {
        LOG("Appel");
        return std::filebuf::overflow(c);
    }
    
    virtual int sync() {
        LOG("Appel");
        return std::filebuf::sync();
    }
    
private:
    
    util::Optional<std::string> _filename;

};


class FileOutputStream: public std::ostream {
    
public:
    
    FileOutputStream(): std::ostream() {
        init(&_buffer);
    }
    
    FileOutputStream(const char *filename, FileOutputStream::openmode mode = FileOutputStream::out):
            std::ostream()
    {
        init(&_buffer);
        open(filename, mode);
    }
    
    void open(const char *filename, FileOutputStream::openmode mode = FileOutputStream::out) {
        // LOG("Ouverture de " << filename);
        if (_buffer.open(filename, mode) == nullptr) {
            setstate(failbit);
        }
    }
    
    void close() {
        // LOG("Fermeture de " << _buffer.getFilename());
        if (_buffer.close() == nullptr) {
            setstate(failbit);
        }
    }
    
    bool is_open() const {
        return _buffer.is_open();
    }
    
    std::string getFilename() const {
        return _buffer.getFilename().orDefault("");
    }

private:
    
    FileStreamBuffer _buffer;
    
};


void testFileOutputStream() {
    LOG("Salut");
    
    char const* filename = "abc.txt";
    
    FileOutputStream s(filename);
    s.exceptions(FileOutputStream::badbit | FileOutputStream::failbit);
    try {
        LOG("Premiere ligne");
        s << "abc" << endl;
        
        LOG("Seconde ligne");
        s << "123\n";
        
        LOG("Troisieme ligne");
        s << "456" << endl;
    
        LOG("Fermeture");
        s.close();
        
        LOG("About stream filename : " << s.getFilename());
    }
    catch (FileOutputStream::failure const& e) {
        LOG("Impossible d'ecrire dans " << filename << " : " << e.what());
        // Dans GCC ca ne marche pas : failure n'herite pas encore de system_error
        // LOG("Error code :" << e.code().value());
    }
}


void testLocale() {
    // Marche pas sur MacOS avec autre chose que LANG=C
    // std::locale::global(std::locale(""));
    
    cout << "Locale is " << cout.getloc().name() << endl;
    cout << 0.9 << endl;
    cout << 8.936f << endl;
}


class File final
{

public:
    
    static size_t const DEFAULT_BUFFER_SIZE = BUFSIZ;
    
    static File open(char const* name, char const* mode, size_t bufferSize = DEFAULT_BUFFER_SIZE) {
        LOG(name << ", mode = " << mode);
        
        FILE* f = fopen(name, mode);
        util::ScopeGuard fileGuard = util::makeFuncGuard(&f, fclose);
        
        if (f == nullptr) {
            fileGuard.dismiss();
            throwSystemError(errno);
        }
        
        File file(f, name, bufferSize);
        fileGuard.dismiss();
        return file;
    }

    File(File const&) = delete;
    
    File(File&& src):
        _file(src._file),
        _name(std::move(src._name)),
        _buffer(src._buffer),
        _closed(src._closed)
    {
        OBJ_LOG("Moving " << &src);
        
        src._file = nullptr;
        src._buffer = nullptr;
        src._closed = true;
    }
    
    ~File() {
        OBJ_LOG("Delete file");
        EXCEPTION_SAFE_BEGIN();
        close();
        delete[] _buffer;
        EXCEPTION_SAFE_END();
    }
    
    File& operator =(File const&) = delete;
    
    File& operator=(File&& src) {
        _file = src._file;
        _name = std::move(src._name);
        _buffer = src._buffer;
        _closed = src._closed;
        
        src._file = nullptr;
        src._buffer = nullptr;
        src._closed = true;
        
        return *this;
    }
    
    void write(std::string const& s) {
        write(s.c_str(), s.size());
    }
    
    void write(char const* s, size_t length) {
        if (fwrite(s, sizeof(char), length, _file) < length) {
            throwSystemError();
        }
    }
    
    void write(char const* s) {
        OBJ_LOG("Write a string " << s);
        if (fputs(s, _file) == EOF) {
            throwSystemError();
        }
    }
    
    void write(char c) {
        if (fputc(c, _file) == EOF) {
            throwSystemError();
        }
    }
    
    void flush() {
        if (fflush(_file) == EOF) {
            throwSystemError();
        }
    }
    
    void truncate(off_t length) {
        flush();
        
        OBJ_LOG("Current position = " << tell() << ", truncate length = " << length);
        
        int fd = getDescriptor();
        if (ftruncate(fd, length) != 0) {
            throwSystemError(errno);
        }
    }
    
    long int tell() {
        long int position = ftell(_file);
        if (position < 0) {
            throwSystemError();
        }
        return position;
    }
    
    void close() {
        OBJ_LOG("Close, state = " << _closed);
        if (!_closed) {
            _closed = true;
            if (fclose(_file) != 0) {
                throwSystemError();
            }
        }
    }
    
    int getDescriptor() {
        if (_closed) {
            throw std::runtime_error("File is closed");
        }
        return fileno(_file);
    }
    
    bool isOpen() const {
        return !_closed;
    }
    
    std::string const& getName() const {
        return _name;
    }

private:
    
    File(FILE* file, std::string const& name, size_t bufferSize):
        _file(file),
        _name(name),
        _buffer(nullptr),
        _closed(false)
    {
        OBJ_LOG("Associate a buffer of " << bufferSize << " bytes");
        
        std::unique_ptr<char[]> buffer(new char[bufferSize]);
        if (setvbuf(_file, buffer.get(), _IOFBF, bufferSize) == 0) {
            _buffer = buffer.release();
        }
        else {
            throw std::runtime_error("Can't set buffer size");
        }
    }
    
    [[ noreturn ]] static void throwSystemError(int errNum) {
        throw std::system_error(errNum, std::system_category());
    }
    
    [[ noreturn ]] void throwSystemError() {
        throwSystemError(errno);
    }
    
    FILE* _file;
    
    std::string _name;
    
    char* _buffer;
    
    bool _closed;

};


void testOutputFile() {
    try {
        File file = File::open("abc.txt", "r", 32 * 1024);
        file.write("123");
        file.truncate(1024);
        file.close();
    }
    catch (std::system_error const& e) {
        LOG("System error : " << e.what());
    }
    catch (std::exception const& e) {
        LOG("Error : " << e.what());
    }
    
    int* i = nullptr;
    {
        i = new int(5);
        util::ScopeGuard guard = util::makeDeleteGuard(&i);
    }
}


int main() {
    // testFileOutputStream();
    // testLocale();
    testOutputFile();
    return 0;
}
