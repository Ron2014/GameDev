#pragma once
//------------------------------------------------------------------------------
/**
@class Future::RefCounted
@ingroup Core
@brief Future's reference-counted base class

The RefCounted class is used together with the Ptr smart-pointer class
to automatically manage the life-time of objects through
reference-counting.

@see Ptr
*/
#include "Core/Ptr.h"
#include "Core/Class.h"
#include "Core/Config.h"
#include <atomic>

namespace Future {

class RefCounted {
    FutureBaseClassDecl(RefCounted);
public:
    /// destructor needs to be virtual
    virtual ~RefCounted() {};
    /// get reference count
    int GetRefCount() const;
    /// add reference
    void addRef();
    /// release reference (calls destructor when ref_count reaches zero)
    void release();

private:
    std::atomic<int> refCount{ 0 };
};

//------------------------------------------------------------------------------
inline void
RefCounted::addRef() {
    this->refCount.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
inline void
RefCounted::release() {
    if (1 == this->refCount.fetch_sub(1, std::memory_order_relaxed)) {
        // destroy() is virtual and provided by the FutureClassDecl macro
        this->destroy();
    }
}

//------------------------------------------------------------------------------
inline int
RefCounted::GetRefCount() const {
    return this->refCount;
}

} // namespace Future
