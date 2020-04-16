#pragma once
#include <cstdint>
#include "Core/Config.h"
#include <stddef.h>
#include <utility>

namespace Future {

class Memory {
public:
    /// allocate a raw chunk of memory
    static void *Alloc(size_t numBytes);
    /// re-allocate a raw chunk of memory
    static void *ReAlloc(void *ptr, size_t numBytes);
    /// free a raw chunk of memory
    static void Free(void *ptr);

    /// fill range of memory with a byte value
    static void Fill(void* ptr, size_t numBytes, unsigned char value);
    /// copy a raw chunk of non-overlapping memory
    static void Copy(const void* from, void* to, size_t numBytes);
    /// move a raw chunk of potentially overlapping memory
    static void Move(const void* from, void* to, size_t numBytes);
    /// fill a chunk of memory with zeros
    static void Clear(void* ptr, size_t numBytes);

    /// align a pointer to size up to FUTURE_MAX_PLATFORM_ALIGN
    static void* Align(void* ptr, int byteSize);
    /// round-up a value to the next multiple of byteSize
    static int RoundUp(int val, int byteSize);
    /// replacement for new() going through Memor

    /// replacement for new() going through Memory::Alloc without overriding new
    template<class TYPE, typename... ARGS> static TYPE* New(ARGS&&... args) {
        TYPE* ptr = (TYPE*)Memory::Alloc(sizeof(TYPE));
        return new(ptr) TYPE(std::forward<ARGS>(args)...);
    };

    /// replacement delete (see Memory::New())
    template<class TYPE> static void Delete(TYPE* ptr) {
        ptr->~TYPE();
        Memory::Free(ptr);
    };

};

//------------------------------------------------------------------------------
inline void*
Memory::Align(void* ptr, int byteSize) {
    intptr_t align = byteSize > FUTURE_MAX_PLATFORM_ALIGN ? FUTURE_MAX_PLATFORM_ALIGN : byteSize;
    intptr_t ptri = (intptr_t)ptr;
    ptri = (ptri + (align - 1)) & ~(align - 1);
    return (void*)ptri;
};

//------------------------------------------------------------------------------
inline int
Memory::RoundUp(int val, int roundTo) {
    return (val + (roundTo - 1)) & ~(roundTo - 1);
}


} // namespace Future