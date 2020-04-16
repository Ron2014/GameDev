#pragma once
#include <type_traits>
#include <algorithm>
#include "Core/Memory/Memory.h"
#include "Core/Assertion.h"

namespace Future {
template<typename DataType>
class MemoryBlock {
public:
    MemoryBlock() {
        static_assert(std::is_trivial<DataType>::value, "DataType must be trivial");
    }

    ~MemoryBlock() {
        this->clear(true);
    }

    DataType *insert(std::size_t index, std::size_t count, bool zero = false) {
        const std::size_t curSize = this->size();
        f_assert(curSize >= index);

        auto available = this->available();
        if (available < count) {
            this->setCapacity(this->size() + count);
        }

        if (index < curSize) {
            Memory::Move(&this->start_[index], &this->start_[index + count], sizeof(DataType) * (curSize - index));
        }

        auto ptr = this->start_ + index;
        if (zero)
            Memory::Fill(ptr, sizeof(DataType)* count, 0);

        this->offset_ += count;
        return ptr;
    }
    
    DataType *pushBack(std::size_t count, bool zero = false) {
        auto available = this->available();
        if (available < count) {
            this->setCapacity(this->size() + count);
        }

        auto ptr = this->offset_;
        this->offset_ += count;

        if (zero) {
            Memory::Fill(ptr, sizeof(DataType) * count, 0);
        }

        return ptr;
    }

    void append(DataType *data, std::size_t count) {
        auto ptr = this->pushBack(count);
        Memory::Copy(data, ptr, sizeof(DataType) * count);
    }

    void append(const DataType &data) {
        auto ptr = this->pushBack(1);
        *ptr = data;
    }

    void remove(std::size_t index, std::size_t count) {
        const std::size_t curSize = this->size();
        if (index <= curSize) {
            auto remaining = curSize - index;
            if (count > remaining)
                count = remaining;

            Memory::Copy(&this->start_[index + count], &this->start_[index], sizeof(DataType) * (remaining - count));
            this->offset_ -= count;
        }
    }

    inline DataType pop_back() {
        if (this->offset_ > this->start_) {
            this->offset_--;
            DataType t = *this->offset_;
            return t;
        }

        f_assert(false);
        DataType t = { 0 };
        return t;
    }

    void setCapacity(std::size_t newCap) {
        const std::size_t cap = this->end_ - this->start_;
        if (newCap == cap) {
            return;
        }

        if (newCap > cap) {
            newCap = (std::max)(newCap, cap << 1);
            newCap = (std::max)(newCap, (std::size_t)4);
            std::size_t pos = this->offset_ - this->start_;
            this->start_ = (DataType*)Memory::ReAlloc(this->start_, sizeof(DataType) * newCap);
            this->end_ = this->start_ + newCap;
            this->offset_ = this->start_ + pos;
        }
    }

    void allocAvailable(std::size_t size) {
        std::size_t available = this->end_ - this->offset_;
        if (available >= size) {
            return;
        }

        this->setCapacity(this->end_ - this->start_ + (size - available));
    }


    void clear(bool release = true) {
        if (this->start_ == nullptr) {
            return;
        }

        if (release) {
            Memory::Free(this->start_);
            this->start_ = nullptr;
            this->end_ = nullptr;
            this->offset_ = nullptr;
        }
        else {
            this->offset_ = this->start_;
        }
    }

    void swap(MemoryBlock &rhs) {
        auto start = this->start_;
        auto end = this->end_;
        auto offset = this->offset_;

        this->start_ = rhs.start_;
        this->end_ = rhs.end_;
        this->offset_ = rhs.offset_;

        rhs.start_ = start;
        rhs.end_ = end;
        rhs.offset_ = offset;
    }

    DataType *get(std::size_t index) {
        const std::size_t curSize = this->size();
        if (index <= curSize) {
            return this->start_ + index;
        }

        return nullptr;
    }


    std::size_t capacity() {
        return this->end - this->start_;
    }

    std::size_t available() {
        return this->end_ - this->offset_;
    }

    std::size_t size() const {
        return this->offset_ - this->start_;
    }

    DataType * memoryPtr(std::size_t index = 0) {
        if (index + this->start_ <= this->end_) {
            return this->start_ + index;
        }
        return nullptr;
    }

    DataType *currentPtr() {
        return this->offset_;
    }

private:
    DataType *start_ = nullptr;
    DataType *end_ = nullptr;
    DataType *offset_ = nullptr;
};
}