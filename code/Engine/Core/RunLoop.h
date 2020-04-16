#pragma once
//------------------------------------------------------------------------------
/**
    @class Future::RunLoop
    @ingroup Core
    @brief universal run-loop object for on-frame callbacks
    
    A runloop object manages a priority-sorted array of callback 
    functions which are called per-frame. By default, each thread
    has a RunLoop object which can be configured through the Core facade
    singleton. Runloops can be nested by adding the Run() function
    of one runloop to another runloop. NOTE that priority values are
    inverted, lower values are called first).
    
    Examples for constructing callbacks:
    
    1. from C function myFunc():

        Callback("name", pri, std::function<void()>(&myFunc);
    2. from an object's method (careful, object must not go out-of-scope
       as long as the callback is added to the RunLoop!

        MyClass myObj;<br>
        Callback("name", pri, std::function<void()>(&MyClass::MyMethod, &myObj));
*/
#include <functional>
#include <map>
#include <set>
#include "SimpleProfiler.h"

namespace Future {

class RunLoop {
public:
    /// runloop Id
    typedef int Id;
    /// invalid runloop Id const
    static const Id InvalidId = 0;
    /// runloop function typedef
    typedef std::function<void()> Func;

    /// constructor
    RunLoop();
    /// destructor
    ~RunLoop();
    
    /// run one frame
    void Run();

    void SetProfiler(SimpleProfiler *profiler_) { this->profiler = profiler_; }
    
    /// add a callback to the run loop, higher priorities run earlier, slow!
    Id Add(const std::string& name, Func func);
    /// remove a callback, slow!
    void Remove(Id);
    /// test if a callback has been attached, slow!
    bool HasCallback(Id) const;
    
private:
    /// add new callbacks that have been added (called at beginning of Run())
    void addCallbacks();
    /// remove callbacks that have been removed (called at end of Run())
    void remCallbacks();
    
    struct item {
        std::string name;
        Func func;
        bool valid;
    };
    
    Id curId;
    std::map<Id, item> callbacks;
    std::map<Id, item> toAdd;
    std::set<Id> toRemove;
    SimpleProfiler *profiler = nullptr;
};
    
} // namespace Future
