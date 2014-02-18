#include <iostream>
#include <cstring>
#include <system_error>
#include <fcntl.h>

#include <boost/numeric/conversion/cast.hpp>

#include "io/memmapped/MemMapped.hpp"
#include "util/ScopeGuard.hpp"

using namespace std;
using namespace io::memmapped;
using namespace util;
using boost::numeric_cast;


struct Tx {
    int a;
    char b;
    long c;
    char d[100];
};


int main() {
    cout << "Test mmap" << endl;
    cout << "Block size = " << sizeof(Tx) << endl;
    
    size_t count = 300;
    size_t length = count * sizeof(Tx);
    
    int fd = open("tx", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        throw system_error(errno, system_category());
    }
    SIMPLE_SCOPE_GUARD(close(fd));

    ftruncate(fd, numeric_cast<off_t>(length));
    
    typedef Tx MappedType;
    
    MemMapped mapped(length, PROT_READ | PROT_WRITE, MAP_SHARED, fd);
    MemView<MappedType> view = mapped.viewAll<MappedType>();
    MemView<MappedType> view2 = mapped.view<MappedType>(1, 10);
    
    for (size_t i = 0; i < view2.length(); i++) {
        MappedType& t = view2.ref(i);
        t.a += numeric_cast<int>(i) + 1;
        t.b = 'G';
        t.c = 10001 + numeric_cast<long>(i);
        strncpy(t.d, "pierre", 6);
    }
    
    // view2.sync(MS_SYNC); // FIXME
    
    cout << "About MemMapped: " << mapped << endl;
    cout << "About MemView: " << view << endl;
        
    return 0;
}
