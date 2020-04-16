#pragma once

#include "Core/Memory/Memory.h"

#define FutureBaseClassDecl(TYPE) \
protected:\
virtual void destroy() {\
    Future::Memory::Delete(this);\
};\
public:\
template<typename... ARGS> static Future::Ptr<TYPE> Create(ARGS&&... args) {\
    return Future::Ptr<TYPE>(Future::Memory::New<TYPE>(std::forward<ARGS>(args)...));\
};

/// declare an Future class without pool allocator (located inside class declaration)
#define FutureClassDecl(TYPE) \
protected:\
virtual void destroy() override {\
    Future::Memory::Delete(this);\
};\
public:\
template<typename... ARGS> static Future::Ptr<TYPE> Create(ARGS&&... args) {\
    return Future::Ptr<TYPE>(Future::Memory::New<TYPE>(std::forward<ARGS>(args)...));\
};

/// add simple RTTI system to a class, inspired by turbobadger's RTTI system
namespace Future {
typedef void* TypeId;
}
#define FutureBaseTypeDecl(CLASS)\
protected:\
    template<class T> static Future::TypeId getTypeId() { static char _type_id; return &_type_id; };\
    virtual bool isOfType(const Future::TypeId t) const { return t == getTypeId<CLASS>(); };\
public:\
    template<class T> bool IsA() const { return this->isOfType(getTypeId<T>()); };\
    template<class T> Future::Ptr<T> DynamicCast() const { return (T*) (this->isOfType(getTypeId<T>()) ? this : nullptr); };\

#define FutureTypeDecl(CLASS, BASECLASS)\
protected:\
    virtual bool isOfType(const Future::TypeId t) const override { return t == getTypeId<CLASS>() ? true : BASECLASS::isOfType(t); };\

