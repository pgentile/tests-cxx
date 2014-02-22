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
#include "util/exceptionsafe.hpp"
#include "io/file/File.hpp"

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


void testOutputFile() {
    try {
        io::file::File file = io::file::File::open("abc.txt", "w", 32 * 1024);
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
}


int main() {
    // testFileOutputStream();
    // testLocale();
    testOutputFile();
    return 0;
}
