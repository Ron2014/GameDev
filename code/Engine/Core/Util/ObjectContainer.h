#pragma once
#include <functional>
#include <map>
#include <vector>
#include <queue>
#include "Core/Memory/Memory.h"
#include "Core/Assertion.h"
#include <stdio.h>
#include <string.h>

namespace Future {
template<class ObjectType, int SegmentBit = 8>
class ObjectContainer {
private:
    struct SegmentInfo {
        int		objectCount = 0;
        ObjectType	**objects = nullptr;
    };

public:
    typedef unsigned int ObjectId;
    typedef std::function<bool(ObjectId objId, ObjectType* obj)> ForEachCallBack;
    typedef std::map<ObjectId, ObjectType*> ObjectMap;

public:
    ObjectContainer(const char* name);
    ~ObjectContainer();

    ObjectType* Get(ObjectId objId);
    void Set(ObjectId, ObjectType* obj);
    void Clear();


    void ForEach(ForEachCallBack callBack);
    int Count();

    const ObjectMap &GetMap() {
        return this->foreachMap_;
    }

private:
    SegmentInfo *getSegment(unsigned int segmentIndex, bool alloc = true);

    void freeBackSegment(unsigned int segmentIndex);

private:
    std::vector<SegmentInfo*>	segments_;
    std::queue<SegmentInfo*>	freeSegments_;
    std::map<ObjectId, ObjectType*> foreachMap_;

    unsigned int segmentMask_;
    unsigned int segmentObjectCount_;

    int total_ = 0;

    std::string name_;
};


template<class ObjectType, int SegmentBit /*= 8*/>
inline
ObjectContainer<ObjectType, SegmentBit>::ObjectContainer(const char* name) {
    static_assert(SegmentBit > 0 && SegmentBit < 32, "SegmentBit must > 0 && < 32");
    this->segmentObjectCount_ = 1 << SegmentBit;
    this->segmentMask_ = this->segmentObjectCount_ - 1;
    this->name_ = name;
}

template<class ObjectType, int SegmentBit /*= 8*/>
inline
ObjectContainer<ObjectType, SegmentBit>::~ObjectContainer() {
    this->Clear();
}

template<class ObjectType, int SegmentBit /*= 8*/>
inline int
ObjectContainer<ObjectType, SegmentBit>::Count() {
    return this->total_;
}

template<class ObjectType, int SegmentBit /*= 8*/>
inline void
ObjectContainer<ObjectType, SegmentBit>::ForEach(ForEachCallBack callBack) {
    auto iter = this->foreachMap_.begin();
    auto end = this->foreachMap_.end();
    for (; iter != end;) {
        auto objId = iter->first;
        auto obj = iter->second;
        ++iter;
        if (!callBack(objId, obj)) {
            return;
        }
    }
}

template<class ObjectType, int SegmentBit /*= 8*/>
inline ObjectType*
ObjectContainer<ObjectType, SegmentBit>::Get(unsigned int objId) {
    auto segmentIndex = objId >> SegmentBit;
    auto objectIndex = objId & this->segmentMask_;

    auto segment = this->getSegment(segmentIndex, false);
    if (segment != nullptr) {
        return segment->objects[objectIndex];
    }
    else {
        return nullptr;
    }
}

template<class ObjectType, int SegmentBit /*= 8*/>
inline void
ObjectContainer<ObjectType, SegmentBit>::Set(unsigned int objId, ObjectType* obj) {
    auto segmentIndex = objId >> SegmentBit;
    auto objectIndex = objId & this->segmentMask_;

    if (obj != nullptr) {
        auto segment = this->getSegment(segmentIndex);
        auto oldObj = segment->objects[objectIndex];
        if (oldObj == nullptr) {
            segment->objectCount++;
            this->total_++;
        }

        segment->objects[objectIndex] = obj;
        this->foreachMap_[objId] = obj;
    }
    else {
        auto segment = this->getSegment(segmentIndex, false);

        if (segment == nullptr) {
            Log::Error("Container %s remove object %u, segment not exist", this->name_.c_str(), objId);
            return;
        }

        auto oldObj = segment->objects[objectIndex];
        if (oldObj == nullptr) {
            Log::Error("Container %s remove object %u, object not exist", this->name_.c_str(), objId);
            return;
        }

        segment->objects[objectIndex] = nullptr;
        segment->objectCount--;
        this->total_--;
        f_assert(segment->objectCount >= 0);

        if (segment->objectCount == 0) {
            this->freeBackSegment(segmentIndex);
        }
        this->foreachMap_.erase(objId);
    }
}

template<class ObjectType, int SegmentBit /*= 8*/>
inline void
ObjectContainer<ObjectType, SegmentBit>::Clear() {
    while (!this->freeSegments_.empty()) {
        auto item = this->freeSegments_.front();
        Memory::Free(item->objects);
        Memory::Delete(item);
        this->freeSegments_.pop();
    }

    for (auto item : this->segments_) {
        if (item != nullptr) {
            Memory::Free(item->objects);
            Memory::Delete(item);
        }
    }
    this->foreachMap_.clear();
    this->segments_.clear();
    this->total_ = 0;
}


template<class ObjectType, int SegmentBit /*= 8*/>
inline typename Future::ObjectContainer<ObjectType, SegmentBit>::SegmentInfo *
Future::ObjectContainer<ObjectType, SegmentBit>::getSegment(unsigned int segmentIndex, bool alloc /*= true*/) {
    if (segmentIndex >= this->segments_.size()) {
        if (!alloc) {
            return nullptr;
        }
        this->segments_.resize(segmentIndex + 1, nullptr);
    }

    SegmentInfo* segment = this->segments_[segmentIndex];

    if (segment == nullptr && alloc) {
        if (!this->freeSegments_.empty()) {
            segment = this->freeSegments_.front();
            this->freeSegments_.pop();
        }
        else {
            segment = Memory::New<SegmentInfo>();
            segment->objects = (ObjectType	**)Memory::Alloc(sizeof(ObjectType*) * this->segmentObjectCount_);
            memset(segment->objects, 0, this->segmentObjectCount_ * sizeof(ObjectType*));
        }
        this->segments_[segmentIndex] = segment;
    }

    return segment;
}

template<class ObjectType, int SegmentBit /*= 8*/>
inline void
Future::ObjectContainer<ObjectType, SegmentBit>::freeBackSegment(unsigned int segmentIndex) {
    f_assert(segmentIndex < this->segments_.size());

    SegmentInfo* segment = this->segments_[segmentIndex];
    f_assert(segment != nullptr);

    this->freeSegments_.push(segment);
    this->segments_[segmentIndex] = nullptr;
}


}