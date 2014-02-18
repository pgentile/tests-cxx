#ifndef IO_MEMMAPPED_MEM_MAPPED_HPP
#define IO_MEMMAPPED_MEM_MAPPED_HPP

#include <cassert>
#include <iostream>
#include <system_error>
#include <type_traits>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <boost/numeric/conversion/cast.hpp>

#include "io/memmapped/memory.hpp"
#include "util/ExceptionSafe.hpp"


namespace io {
namespace memmapped {
    
    using namespace std;
    using boost::numeric_cast;
    
    template<typename T>
    class MemView;
    
    class MemMapped {

    public:

        MemMapped(size_t length, int protections, int flags, int fd, off_t offset = 0):
            _start(nullptr),
            _end(nullptr),
            _length(length)
        {
            _start = mmap(nullptr, length, protections, flags, fd, offset);
            if (_start == MAP_FAILED) {
                throw system_error(errno, system_category());
            }
            _end = reinterpret_cast<char*>(_start) + length;
        }
        
        MemMapped(MemMapped const &) = delete;
        
        MemMapped(MemMapped&& src):
            _start(src._start),
            _end(src._end),
            _length(src._length)
        {    
            src._start = nullptr;
            src._end = nullptr;
        }

        ~MemMapped() {
            EXCEPTION_SAFE_BEGIN();
            if (_start != nullptr) {
                if (munmap(_start, _length) == -1) {
                    throw system_error(errno, system_category());
                }
            }
            EXCEPTION_SAFE_END();
        }
        
        MemMapped& operator =(MemMapped const &) = delete;
        
        MemMapped& operator =(MemMapped&& src) {
            _start = src._start;
            _end = src._end;
            _length = src._length;
            
            src._start = nullptr;
            src._end = nullptr;
            
            return *this;
        }

        void* start() const {
            return _start;
        }

        void* end() const {
            return _end;
        }

        size_t length() const {
            return _length;
        }

        template<typename T>
        T* addr(size_t offset) const {
            T* pt = reinterpret_cast<T*>(_start) + offset;
            assert(_start <= pt && pt <= _end);
            return pt;
        }

        template<typename T>
        MemView<typename std::enable_if<std::is_pod<T>::value, T>::type> view(void* start, void* end) {
            assert(start < end);
            assert(_start <= start && end <= _end);
            T* startPt = reinterpret_cast<T*>(start);
            T* endPt = reinterpret_cast<T*>(end);
            return MemView<T>(*this, startPt, endPt);
        }
        
        template<typename T>
        MemView<typename std::enable_if<std::is_pod<T>::value, T>::type> view(size_t offset, size_t length) {
            T* start = addr<T>(offset);
            T* end = start + length;
            return view<T>(start, end);
        }
        
        template<typename T>
        MemView<T> viewAll() {
            return view<T>(_start, _end);
        }

    private:

        void* _start;
        
        void* _end;
        
        size_t _length;

    };


    std::ostream& operator <<(std::ostream& out, MemMapped const& mapped) {
        out << "MemMapped(length = " << mapped.length()
            << ", start = " << mapped.start()
            << ", end  " << mapped.end() << ")";
        return out;
    }
    

    template<typename T>
    class MemView
    {

    public:

        typedef T type;

        MemView(MemMapped & mapped, T* start, T* end):
            _mapped(mapped),
            _start(start),
            _end(end),
            _length(numeric_cast<size_t>(end - start))
        {
        }

        MemView(MemView const &) = delete;
        
        MemView(MemView&& src) = default;

        ~MemView() {
        }

        MemView& operator =(MemView const &) = delete;
        
        MemView& operator =(MemView&& src) = default;

        void sync(int flags) {
            memory::sync(_start, _length * sizeof(T), flags);
        }

        T* start() const {
            return _start;
        }

        T* end() const {
            return _end;
        }
        
        size_t length() const {
            return _length;
        }
        
        T* addr(size_t offset) const {
            return _start + offset;
        }
        
        T& ref(size_t offset) const {
            return *addr(offset);
        }
        

    private:

        MemMapped& _mapped;

        T* const _start;
        
        T* const _end;

        size_t const _length;

    };


    template<typename T>
    std::ostream& operator <<(std::ostream& out, MemView<T> const& view) {
        out << "MemView(length = " << view.length()
            << ", start = " << static_cast<void const *>(view.start())
            << ", end = " << static_cast<void const *>(view.end()) << ")";
        return out;
    }

} // namespace memmapped
} // namespace io

#endif
