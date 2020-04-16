#pragma once
//------------------------------------------------------------------------------
/**
    @defgroup Core Core
    @brief Future core functionality

    @class Future::Core
    @ingroup Core
    @brief Core module facade
*/
#include "Core/RunLoop.h"
#include "Synchronizator.h"

namespace Future {

class Core {
public:
    /// setup the Core module
    static void Setup();
    /// discard the Core module
    static void Discard();
    /// check if Core module has been setup
    static bool IsValid();
    
    /// get pointer to the per-thread 'before-frame' runloop
    static class RunLoop* PreRunLoop();
    /// get pointer to the per-thread 'after-frame' runloop
    static class RunLoop* PostRunLoop();

    /// called when a thread is entered
    static void EnterThread();
    /// called before a thread is left
    static void LeaveThread();
    /// test if we are on the main thread
    static bool IsMainThread();

	static Synchronizator* GetSynchronizator();
};

} // namespace Future
