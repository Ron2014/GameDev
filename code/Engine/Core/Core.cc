//------------------------------------------------------------------------------
//  Core.cc
//------------------------------------------------------------------------------
#include "StdFuture.h"
#include "Core.h"
#include "Core/RunLoop.h"
#include "Core/Thread/ThreadLocalPtr.h"

#include <thread>

namespace Future {
    
namespace {
    FUTURE_THREADLOCAL_PTR(RunLoop) threadPreRunLoop = nullptr;
    FUTURE_THREADLOCAL_PTR(RunLoop) threadPostRunLoop = nullptr;
    struct _state {
        std::thread::id mainThreadId;
		Synchronizator synchronizator;
    };
    _state* state = nullptr;
}

//------------------------------------------------------------------------------
void
Core::Setup() {
    f_assert_dbg(!IsValid());
    f_assert_dbg(nullptr == threadPreRunLoop);
    f_assert_dbg(nullptr == threadPostRunLoop);
    state = Memory::New<_state>();
    state->mainThreadId = std::this_thread::get_id();
    threadPreRunLoop = Memory::New<RunLoop>();
    threadPostRunLoop = Memory::New<RunLoop>();
}

//------------------------------------------------------------------------------
void
Core::Discard() {
    f_assert(IsValid());
    f_assert(threadPreRunLoop);
    f_assert(threadPostRunLoop);
    Memory::Delete<RunLoop>(threadPreRunLoop);
    Memory::Delete<RunLoop>(threadPostRunLoop);
    Memory::Delete(state);
    threadPreRunLoop = nullptr;
    threadPostRunLoop = nullptr;
    state = nullptr;

    // do NOT destroy the thread-local string atom table to
    // ensure that string atom data pointers still point to valid data!!!    
}

//------------------------------------------------------------------------------
bool
Core::IsValid() {
    return nullptr != state;
}

//------------------------------------------------------------------------------
RunLoop*
Core::PreRunLoop() {
    f_assert(threadPreRunLoop);
    return threadPreRunLoop;
}

//------------------------------------------------------------------------------
RunLoop*
Core::PostRunLoop() {
    f_assert(threadPostRunLoop);
    return threadPostRunLoop;
}

//------------------------------------------------------------------------------
bool
Core::IsMainThread() {
    #if FUTURE_HAS_THREADS
    f_assert_dbg(IsValid());
    return state->mainThreadId == std::this_thread::get_id();
    #else
    return true;
    #endif
}

Synchronizator* 
Core::GetSynchronizator() {
	f_assert(IsValid());
	return &state->synchronizator;
}

//------------------------------------------------------------------------------
void
Core::EnterThread() {
    #if FUTURE_HAS_THREADS
    f_assert(nullptr == threadPreRunLoop);
    f_assert(nullptr == threadPostRunLoop);
    threadPreRunLoop = Memory::New<RunLoop>();
    threadPostRunLoop = Memory::New<RunLoop>();
    #endif
}

//------------------------------------------------------------------------------
void
Core::LeaveThread() {
    #if FUTURE_HAS_THREADS
    f_assert(threadPreRunLoop);
    f_assert(threadPostRunLoop);
    Memory::Delete<RunLoop>(threadPreRunLoop);
    Memory::Delete<RunLoop>(threadPostRunLoop);
    threadPreRunLoop = nullptr;
    threadPostRunLoop = nullptr;

    // do NOT destroy the thread-local string atom table to
    // ensure that string atom data pointers still point to valid data
    #endif
}

} // namespace Future
