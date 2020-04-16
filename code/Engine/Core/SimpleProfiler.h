#pragma once


#include <map>

namespace Future {

class SimpleProfiler {
public:
    SimpleProfiler() { }

    virtual uint64_t ProfileStart() = 0;
    virtual void ProfileEnd(const std::string &name, uint64_t beginTime) = 0;
    virtual void ProfileEnd(const char* name, uint64_t beginTime) = 0;
};
}