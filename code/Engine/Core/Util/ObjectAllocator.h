#pragma once
#include <vector>
#include <type_traits>
#include <utility>

#include "Core/Assertion.h"

namespace Future {


template<typename ObjectType, bool Reusable = true, int AllocOnce = 256>
class ObjectAllocator {
private:
    template<typename T>
    typename std::enable_if<std::is_trivial<T>::value, bool>::type
    s_IsTrivial() {
        return true;
    }

    template<typename T>
    typename std::enable_if<!std::is_trivial<T>::value, bool>::type
    s_IsTrivial() {
        return false;
    }

private:
    const std::size_t allocSize_ = sizeof(DataBlock) + sizeof(ObjectType) * AllocOnce;

private:
    struct DataBlock {
        DataBlock   *Prev;
    };

public:
    ~ObjectAllocator() {
        this->freeAll();
    }

    bool isTrivial() {
        return s_IsTrivial<ObjectType>();
    }

    void freeAll() {
        f_assert(this->used_ == 0);
        DataBlock *block = this->block_;
        DataBlock *prev = nullptr;

        while (block) {
            prev = block->Prev;
            Memory::Free(block);
            block = prev;
        }

        this->items_.clear();
        this->block_ = nullptr;
        this->used_ = 0;
    }

    ObjectType *allocObject() {
        ObjectType * mem = this->_allocObject();
        return mem;
    }

    void freeObject(ObjectType *item) {
        f_assert(this->used_ > 0);
        f_assert(item != nullptr);
        this->_freeObject(item);
        this->items_.push_back(item);
        this->used_--;
    }

    template<typename... ARGS>
    ObjectType *allocObject(ARGS&&... args) {
        static_assert(!Reusable, "can't alloc reusable object with args");
        ObjectType * mem = this->_allocObject(std::forward<ARGS>(args)...);
        return new(mem) ObjectType(std::forward<ARGS>(args)...);
    }

private:
    template<typename T>
    typename std::enable_if<std::is_trivial<T>::value, void>::type
    _freeObject(T *item) {
        if (!Reusable) {
            Memory::Fill(item, sizeof(T), 0);
        }

    }

    template<typename T>
    typename std::enable_if<!std::is_trivial<T>::value, void>::type
    _freeObject(T *item) {
        if (!Reusable) {
            item->~T();
        }
    }

    template<typename... ARGS>
    ObjectType *_allocObject(ARGS&&... args) {
        if (this->items_.empty()) {
            DataBlock *block = (DataBlock*)Memory::Alloc(allocSize_);

            if (this->isTrivial() || !Reusable) {
                Memory::Fill(block, allocSize_, 0);
            }

            this->items_.reserve(AllocOnce);

            ObjectType *item = (ObjectType *)(block + 1);

            bool ctor = !this->isTrivial() && Reusable;
            for (int i = 0; i < AllocOnce; ++i) {
                if (ctor) {
                    item = new(item) ObjectType(std::forward<ARGS>(args)...);
                }
                this->items_.push_back(item ++);
            }

            block->Prev = this->block_;
            this->block_ = block;
            this->alloced_ += AllocOnce;
        }

        this->used_++;

        auto ret = this->items_.back();
        this->items_.pop_back();
        return ret;
    }


private:
    int used_ = 0;
    int alloced_ = 0;

    std::vector<ObjectType*> items_;
    DataBlock *block_ = nullptr;
};

}