//------------------------------------------------------------------------------
//  RunLoop.cc
//------------------------------------------------------------------------------
#include "StdFuture.h"
#include "RunLoop.h"

namespace Future {

//------------------------------------------------------------------------------
RunLoop::RunLoop() :
curId(InvalidId)
{
    // empty
}

//------------------------------------------------------------------------------
RunLoop::~RunLoop() {
    // empty
}

//------------------------------------------------------------------------------
void
RunLoop::Run() {
    this->remCallbacks();
    this->addCallbacks();

    uint64_t begin_time = 0;

    for (const auto& entry : this->callbacks) {
        if (entry.second.valid) {
            if (this->profiler) {
                begin_time = this->profiler->ProfileStart();
            }

            entry.second.func();

            if (begin_time != 0 && this->profiler) {
                this->profiler->ProfileEnd(entry.second.name, begin_time);
                begin_time = 0;
            }
        }
    }
    this->remCallbacks();
    this->addCallbacks();
}

//------------------------------------------------------------------------------
bool
RunLoop::HasCallback(Id id) const {
    return this->callbacks.find(id) != this->callbacks.end()
        || this->toAdd.find(id) != this->callbacks.end();
}

//------------------------------------------------------------------------------
/**
 NOTE: the callback function will not be added immediately, but at the
 start or end of the Run function.
*/
RunLoop::Id
RunLoop::Add(const std::string& name, Func func) {
    Id newId = ++this->curId;
    this->toAdd.emplace(newId, item{ name, func, false});
    return newId;
}

//------------------------------------------------------------------------------
/**
 NOTE: the callback function not be removed immediately, but at the 
 start or end of the Run function.
*/
void
RunLoop::Remove(Id id) {
    f_assert_dbg(this->toRemove.find(id) == this->toRemove.end());
    f_assert_dbg(this->callbacks.find(id) != this->callbacks.end() || this->toAdd.find(id) != this->toAdd.end());
    this->toRemove.insert(id);
}

//------------------------------------------------------------------------------
void
RunLoop::addCallbacks() {
    for (auto& entry : this->toAdd) {
        item& item = entry.second;
        item.valid = true;
        this->callbacks.emplace(entry.first, item);
    }
    this->toAdd.clear();
}

//------------------------------------------------------------------------------
void
RunLoop::remCallbacks() {
    for (Id id : this->toRemove) {
        if (this->callbacks.find(id) != this->callbacks.end()) {
            this->callbacks.erase(id);
        }
        else if (this->toAdd.find(id) != this->callbacks.end()) {
            this->toAdd.erase(id);
        }
    }
    this->toRemove.clear();
}

} // namespace Future
