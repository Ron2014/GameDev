#include "Core/Assertion.h"
#include "Core/Core.h"

#include "Tick.h"
#include "Tick/tickMgr.h"


namespace Future {

Tick::_state* Tick::state = nullptr;

//------------------------------------------------------------------------------
void
Tick::Setup() {
	f_assert_dbg(!IsValid());
    state = Memory::New<_state>();
    state->tickMgr = Memory::New<_priv::tickMgr>();

    state->tickMgr->Setup();

    state->runLoopId = Core::PreRunLoop()->Add("Tick", [] {
        state->tickMgr->PushTicks();
    });
}

//------------------------------------------------------------------------------
void
Tick::Discard() {
	f_assert_dbg(IsValid());
    Core::PreRunLoop()->Remove(state->runLoopId);
    state->tickMgr->Discard();
    Memory::Delete(state->tickMgr);
    state->tickMgr = nullptr;

    Memory::Delete(state);
    state = nullptr;
}

//------------------------------------------------------------------------------
bool
Tick::IsValid() {
    return state != nullptr;
}

unsigned int 
Tick::RegisterTick(uint64_t interval, TickFun func, const char* tickName) {
    f_assert_dbg(IsValid());
	return state->tickMgr->RegisterTick(interval, func, tickName);
}
void 
Tick::UnregisterTick(unsigned int id) {
    f_assert_dbg(IsValid());
    state->tickMgr->UnregisterTick(id);
}

unsigned int 
Tick::GetTickCount() {
	return state->tickMgr->GetTickCount();
}


void 
Tick::PushTicks() {
    state->tickMgr->PushTicks();
}

}