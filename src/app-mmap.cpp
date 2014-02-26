#include <iostream>
#include <cstring>
#include <system_error>
#include <type_traits>
#include <cstddef>

#include <boost/numeric/conversion/cast.hpp>

#include "io/memmapped/MemMapped.hpp"
#include "io/file/File.hpp"
#include "util/ScopeGuard.hpp"

using namespace std;
using namespace io::memmapped;
using namespace io::file;
using namespace util;
using boost::numeric_cast;


#define LOG(MSG) (cout << __func__ << ", L" << __LINE__ << ": " << MSG << endl)


struct Tx {
    int a;
    char b;
    long c;
    char d[100];
};


void simpleTest() {
    typedef Tx MappedType;
    
    LOG("sizeof(File) = " << sizeof(File));
    LOG("sizeof(MemMapped) = " << sizeof(MemMapped));
    LOG("sizeof(MemView) = " << sizeof(MemView<MappedType>));
    
    LOG("Test mmap");
    LOG("Block size = " << sizeof(MappedType));
    
    size_t count = 300;
    size_t length = count * sizeof(MappedType);
    
    try {
        File file = File::open("tx", "w+");
        file.truncate(numeric_cast<off_t>(length));
    
        // Ce qui est fabrique par ce bloc sera efface
        {
            MappedType t;
            memset(&t, '\0', sizeof(MappedType));
        
            t.a = 8956;
            t.b = 'Z';
            t.c = 1024;
            strncpy(t.d, "1234", 4);
        
            file.write(t);
            file.flush();
        
            LOG("Position courante : " << file.tell());
        }
    
        MemMapped mapped(length, PROT_READ | PROT_WRITE, MAP_SHARED, file.getDescriptor());
        MemView<MappedType> view = mapped.viewAll<MappedType>();
        MemView<MappedType> view2 = mapped.view<MappedType>(0, 10);
    
        for (size_t i = 0; i < view2.length(); i++) {
            MappedType& t = view2.ref(i);
            t.a += numeric_cast<int>(i) + 1;
            t.b = 'G';
            t.c = 10001 + numeric_cast<long>(i);
            strncpy(t.d, "123456", 6);
        }
    
        // view2.sync(MS_SYNC); // FIXME
    
        LOG("About MemMapped: " << mapped);
        LOG("About MemView: " << view);
    }
    catch (exception const& e) {
        LOG("Exception attrapee : " << e.what());
    }
}


struct Header {
    
};


int main() {
    typedef typename aligned_storage<sizeof(long), alignof(max_align_t)>::type VersionStorage;
    typedef typename aligned_storage<100, alignof(max_align_t)>::type HeaderStorage;
    typedef typename aligned_storage<100, alignof(max_align_t)>::type BlockStorage;
    
    constexpr size_t const count = 10000;
    constexpr size_t const fileLength = sizeof(VersionStorage) + sizeof(HeaderStorage) + sizeof(BlockStorage) * count;
    
    LOG("Tx file size = " << fileLength);
    LOG("Size = " << sizeof(VersionStorage) << ", align = " << alignof(VersionStorage));
    LOG("Size = " << sizeof(HeaderStorage) << ", align = " << alignof(HeaderStorage));
    LOG("Size = " << sizeof(BlockStorage) << ", align = " << alignof(BlockStorage));
    
    File file = File::open("tx", "w+");
    file.truncate(numeric_cast<off_t>(fileLength));
    
    MemMapped mapped(fileLength, PROT_READ | PROT_WRITE, MAP_SHARED, file.getDescriptor());
    LOG("mapped = " << mapped);
    
    MemView<VersionStorage> versionView = mapped.view<VersionStorage>(0, 1);
    LOG("versionView = " << versionView);
    
    long* version = reinterpret_cast<long*>(versionView.start());
    *version = 10;
    
    MemView<HeaderStorage> headerView = mapped.viewFrom<HeaderStorage>(versionView.end(), 1);
    LOG("headerView = " << headerView);
    
    char* header = reinterpret_cast<char*>(headerView.start());
    strcpy(header, "MAGIC TX");
    
    MemView<BlockStorage> blockView = mapped.viewFrom<BlockStorage>(headerView.end(), count);
    LOG("blockView = " << blockView);
    
    return 0;
}
