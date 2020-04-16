#pragma once
#include "Core/RefCounted.h"
#include "Core/RunLoop.h"

namespace Future {

namespace _priv {
class tickMgr;
}

class Tick {
public:
    typedef std::function<void()> TickFun;
public:
	/// setup the Tick module
    static void Setup();
    /// discard the Tick module
    static void Discard();
    /// check if Tick module is valid
    static bool IsValid();
    
    static unsigned int RegisterTick(uint64_t interval, TickFun func, const char* tickName);
	static void UnregisterTick(unsigned int id);

	static unsigned int GetTickCount();

    static void PushTicks();

private:
    struct _state {
        RunLoop::Id runLoopId = RunLoop::InvalidId;
        _priv::tickMgr *tickMgr = nullptr;
    };
    static _state* state;
};

class TickHelperObject : public RefCounted {
    FutureClassDecl(TickHelperObject);
public:
    TickHelperObject(unsigned int id) :
        id_(id){
    }

    ~TickHelperObject() {
        Tick::UnregisterTick(id_);
        id_ = 0;
    }
private:
    unsigned int id_;
};
}