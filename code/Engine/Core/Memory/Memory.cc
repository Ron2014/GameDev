#include "Memory.h"
#include <cstring>
#include <memory>

namespace Future {

void *
Memory::Alloc(size_t numBytes) {
    void *ptr = std::malloc(numBytes);
    return ptr;
}

void *
Memory::ReAlloc(void *ptr, size_t numBytes) {
    return std::realloc(ptr, numBytes);
}

void
Memory::Free(void *ptr) {
    std::free(ptr);
}

void
Memory::Fill(void* ptr, size_t numBytes, unsigned char value) {
    std::memset(ptr, value, numBytes);
}

void
Memory::Copy(const void* from, void* to, size_t numBytes) {
    std::memcpy(to, from, numBytes);
}

void
Memory::Move(const void* from, void* to, size_t numBytes) {
    std::memmove(to, from, numBytes);
}

void
Memory::Clear(void* ptr, size_t numBytes) {
    std::memset(ptr, 0, numBytes);
}

} // namespace Future