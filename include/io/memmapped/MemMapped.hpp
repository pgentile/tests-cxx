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
    
    template<typename T>
    class MemView;
    
    class MemMapped {

    public:

        MemMapped(size_t length, int protections, int flags, int fd, off_t offset = 0):
            _length(length),
            _start(nullptr)
        {
            _start = mmap(nullptr, length, protections, flags, fd, offset);
            if (_start == MAP_FAILED) {
                throw system_error(errno, system_category());
            }
        }
        
        MemMapped(MemMapped const &) = delete;
        
        MemMapped(MemMapped&& src):
            _length(src._length),
            _start(src._start)
        {
            src._start = nullptr;
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
            _length = src._length;
            _start = src._start;
            
            src._start = nullptr;
            
            return *this;
        }

        void* start() const {
            return _start;
        }

        void* end() const {
            return reinterpret_cast<char*>(_start) + _length;
        }

        template<typename T>
        T* addr(size_t offset) const {
            return reinterpret_cast<T*>(_start) + offset;
        }

        size_t length() const {
            return _length;
        }
        
        template<typename T>
        MemView<typename std::enable_if<std::is_pod<T>::value, T>::type> view(size_t offset, size_t length) {
            return MemView<T>(*this, offset, length);
        }
        
        template<typename T>
        MemView<T> viewAll() {
            assert(_length % sizeof(T) == 0);
            return view<T>(0, _length / sizeof(T));
        }

    private:

        size_t _length;

        void* _start;

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

        MemView(MemMapped & mapped, size_t offset, size_t length):
            _mapped(mapped),
            _start(mapped.addr<T>(offset)),
            _length(length)
        {
            assert(end() <= mapped.end());
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
            return _start + _length;
        }

        T* addr(size_t offset) const {
            return _start + offset;
        }
        
        T& ref(size_t offset) const {
            return *addr(offset);
        }
        
        MemView<T> view(ptrdiff_t offset, size_t length) {
            ptrdiff_t currentOffset = reinterpret_cast<char*>(_start) - reinterpret_cast<char*>(_mapped.start());
            MemView<T> childView = MemView<T>(_mapped, currentOffset + offset, length);
            assert(childView.end() <= end());
            return childView;
        }
        
        size_t length() const {
            return _length;
        }

    private:

        MemMapped& _mapped;

        T* const _start;

        size_t _length;

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
