#pragma once
#include "Core/Config.h"
#include "ThreadLocalData.h"

#if FUTURE_THREADLOCAL_PTHREAD
// platform only has pthread-keys, not compiler keyword
#define FUTURE_THREADLOCAL_PTR(T) Future::ThreadLocalPtr<T>
#elif FUTURE_COMPILER_HAS_THREADLOCAL
#if FUTURE_WINDOWS && defined(_MSC_VER)
// on Windows, use __declspec(thread)
#define FUTURE_THREADLOCAL_PTR(T) __declspec(thread) T*
#else
// on GCC/Clang, use __thread
#define FUTURE_THREADLOCAL_PTR(T) __thread T*
#endif
#else
// no threading support at all (e.g. emscripten)
#define FUTURE_THREADLOCAL_PTR(T) T*
#endif

#if FUTURE_THREADLOCAL_PTHREAD
namespace Future {

template<class T> class ThreadLocalPtr {
public:
    /// constructor
    ThreadLocalPtr();
    /// construct with ptr
    ThreadLocalPtr(T* p);
    /// assign pointer
    void operator=(T* p);
    /// get ptr
    operator T*() const;
    /// operator->
    T* operator->() const;
    /// test if contains valid ptr
    explicit operator bool() const;

private:
    int slotIndex;
};

//------------------------------------------------------------------------------
template<class T>
ThreadLocalPtr<T>::ThreadLocalPtr() {
    ThreadLocalData::SetupOnce();
    this->slotIndex = ThreadLocalData::Alloc();
}

//------------------------------------------------------------------------------
template<class T>
ThreadLocalPtr<T>::ThreadLocalPtr(T* p) {
    ThreadLocalData::SetupOnce();
    this->slotIndex = ThreadLocalData::Alloc();
    ThreadLocalData::Set(this->slotIndex, (void*)p);
}

//------------------------------------------------------------------------------
template<class T> void
ThreadLocalPtr<T>::operator=(T* p) {
    ThreadLocalData::Set(this->slotIndex, (void*)p);
}

//------------------------------------------------------------------------------
template<class T>
ThreadLocalPtr<T>::operator T*() const {
    return (T*)ThreadLocalData::Get(this->slotIndex);
}

//------------------------------------------------------------------------------
template<class T> T*
ThreadLocalPtr<T>::operator->() const {
    return *this;
}

//------------------------------------------------------------------------------
template<class T>
ThreadLocalPtr<T>::operator bool() const {
    return nullptr != ThreadLocalData::Get(this->slotIndex);
}

} // namespace Future
#endif // FUTURE_THREADLOCAL_PTHREAD