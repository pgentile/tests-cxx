#include <iostream>
#include <fcntl.h>

#include <boost/numeric/conversion/cast.hpp>

#include "io/memmapped/MemMapped.hpp"
#include "util/ScopeGuard.hpp"

using namespace std;
using namespace io::memmapped;
using namespace util;
using boost::numeric_cast;


int main() {
    cout << "Test mmap" << endl;
    
    size_t count = 10;
    size_t length = count * sizeof(char);
    
    int fd = open("bite", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        throw system_error(errno, system_category());
    }
    ScopeGuard fdGuard = makeFuncGuard(&fd, close);

    ftruncate(fd, numeric_cast<off_t>(length));
    
    MemMapped mapped(length, PROT_READ | PROT_WRITE, MAP_SHARED, fd);
    MemView<char> view(mapped, 0, count);
    
    char* addr = view.start();
    *addr = 'A';
    
    addr = view.end() - 1;
    *addr = 'Z';
    
    cout << "start, incusive = " << static_cast<void*>(view.start()) << endl;
    cout << "end, exclusive  = " << static_cast<void*>(view.end()) << endl;
    cout << "size            = " << (view.end() - view.start()) << endl;
    
    //view.sync(MS_SYNC);
    
    cout << "About MemMapped: " << mapped << endl;
    cout << "About MemView: " << view << endl;
        
    return 0;
}
