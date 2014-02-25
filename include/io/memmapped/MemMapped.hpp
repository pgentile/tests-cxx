#ifndef IO_MEMMAPPED_MEM_MAPPED_HPP
#define IO_MEMMAPPED_MEM_MAPPED_HPP

#include <cstddef>
#include <cassert>
#include <iostream>
#include <type_traits>
#include <fcntl.h>
#include <sys/mman.h>

#include <boost/numeric/conversion/cast.hpp>

#include "io/memmapped/memory.hpp"


namespace io {
namespace memmapped {
    
    using namespace std;
    
    template<typename T>
    class MemView;
    
    class MemMapped {

    public:

        MemMapped(size_t length, int protections, int flags, int fd, off_t offset = 0);
        
        MemMapped(MemMapped const &) = delete;
        
        MemMapped(MemMapped&& src);

        ~MemMapped() noexcept;
        
        MemMapped& operator =(MemMapped const &) = delete;
        
        MemMapped& operator =(MemMapped&& src);

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


    template<typename T>
    class MemView
    {

    public:

        typedef T type;

        MemView(MemMapped & mapped, T* start, T* end):
            _mapped(mapped),
            _start(start),
            _end(end),
            _length(boost::numeric_cast<size_t>(end - start))
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

} // namespace memmapped
} // namespace io


namespace std {
    
    ostream& operator <<(ostream& out, io::memmapped::MemMapped const& mapped);


template<typename T>
ostream& operator <<(ostream& out, io::memmapped::MemView<T> const& view) {
    out << "MemView(length = " << view.length()
        << ", start = " << static_cast<void const *>(view.start())
        << ", end = " << static_cast<void const *>(view.end()) << ")";
    return out;
}
    
} // namespace std

#endif
