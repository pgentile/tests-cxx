#include <iostream>

#include <boost/numeric/conversion/cast.hpp>

#include "io/memmapped/MemMapped.hpp"

using namespace std;
using namespace io::memmapped;
using boost::numeric_cast;


int main() {
    cout << "Test mmap" << endl;
    
    size_t count = 10;
    size_t length = count * sizeof(char);
    
    FileDescriptor descriptor("bite", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(descriptor.fd(), numeric_cast<off_t>(length));
    
    MemMapped mapped(length, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor.fd());
    MemView<char> view(
        mapped,
        0,
        count
    );
    view.sync(MS_SYNC | MS_INVALIDATE);
    
    cout << "About MemMapped: " << mapped << endl;
    cout << "About MemView: " << view << endl;
        
    return 0;
}
