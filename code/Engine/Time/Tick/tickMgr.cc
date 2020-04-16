#include <inttypes.h>

#include "Core/Assertion.h"
#include "Time/Clock.h"

#include "tickMgr.h"

namespace Future {
namespace _priv {

void
tickMgr::Setup() {
    f_assert_dbg(!this->IsValid());

    union { struct _timer_node *vn; struct _timer_root *vr; } uv;

    uv.vr = &this->root;

    this->nodes[0] = uv.vn;
    this->nodes[1] = &node2;
    this->nodes[2] = &node3;
    this->nodes[3] = &node4;
    this->nodes[4] = &node5;

	this->lastMillisec = Clock::GetRealTime();
    this->isValid = true;
}


void
tickMgr::Discard() {
    f_assert_dbg(this->IsValid());

    this->isValid = false;

    for (int slot = 0; slot < TickEnums::RootSize; slot++) {
        auto &lst = this->root.slots[slot];
		while (!lst.empty()) {
			_timer_context *context = lst.front();
			this->removeContext(context);
			lst.pop_front();
        }
    }

    for (int slot = 0; slot < TickEnums::NodeSize; slot++) {
        for (int nodeId = 1; nodeId < TickEnums::TotalNode; nodeId++) {
            auto &lst = this->nodes[nodeId]->slots[slot];
			while (!lst.empty()) {
				_timer_context *context = lst.front();
				this->removeContext(context);
				lst.pop_front();
            }
        }
    }

    this->contexts.Clear();
}

bool
tickMgr::IsValid() const {
    return this->isValid;
}

unsigned int
tickMgr::RegisterTick(uint64_t interval, TickFun func, const char* tickName) {
    unsigned int id = this->genTickId();

    _timer_context *context = Memory::New<_timer_context>();

    context->id = id;
    context->interval = interval;
    context->expires = this->jiffies + interval;
    context->func = func;
	context->name = tickName;

    this->addContext(context);
    this->contexts.Set(id, context);
    return id;
}

void 
tickMgr::UnregisterTick(unsigned int id) {
    _timer_context *context = this->getContext(id);
    if (context == nullptr) {
        Log::Error("[tickMgr::UnregisterTick][tickId: %u]: not found ", id);
        return;
    }

    if (context->terminate) {
        return;
    }

    context->terminate = true;
    //context->func = nullptr;
	this->dirty_contexts.insert(id);
}

uint64_t
tickMgr::lapTime() {
    uint64_t millisec_now = Clock::GetRealTime();
    uint64_t  dur = millisec_now - lastMillisec;
    lastMillisec = millisec_now;
    return dur;
}


void
tickMgr::PushTicks() {
    uint64_t millisec = this->lapTime();
    while (millisec > 0) {
        unsigned int slot = this->jiffies & TickEnums::RootMask;
        if (slot == 0) {
            unsigned char nodeId = 1;
            do {
                this->cascadeTimers(nodeId);
            } while (this->getNodeSlot(nodeId) == 0 && ++nodeId < TickEnums::TotalNode);
        }

        auto &lst = this->root.slots[slot];
		while (!lst.empty()) {
			_timer_context *context = lst.front();

            if (context->terminate) {
                this->removeContext(context);
            }
            else {
                uint64_t beginTime = 0;

                context->func();

                if (context->terminate) {
                    this->removeContext(context);
                }
                else {
                    context->expires += context->interval;
                    this->addContext(context);
                }
            }
			lst.pop_front();
        }
        this->jiffies++;
        millisec--;
    }

	if (!this->dirty_contexts.empty()) {
		for (auto &id : this->dirty_contexts) {
			_timer_context *context = this->getContext(id);
			if (context != nullptr && context->expires) {
				context->func = nullptr;
			}
		}
		this->dirty_contexts.clear();
	}
}

unsigned int
tickMgr::GetTickCount() {
    return this->contexts.Count();
}

void 
tickMgr::addContext(_timer_context *context) {
    uint64_t expires = context->expires;
    uint64_t timeLeft = expires < this->jiffies ? 0 : expires - this->jiffies;

    unsigned int node = 0;
    unsigned int slot = 0;

    if (timeLeft < TickEnums::RootSize) {
        node = 0;
        slot = expires & TickEnums::RootMask;
    }
    else if (timeLeft < ((uint64_t)1 << (TickEnums::RootBits + TickEnums::NodeBits))) {
        node = 1;
        slot = (expires >> TickEnums::RootBits) & TickEnums::NodeMask;
    }
	else if (timeLeft < ((uint64_t)1 << (TickEnums::RootBits + TickEnums::NodeBits * 2))) {
        node = 2;
        slot = (expires >> (TickEnums::RootBits + TickEnums::NodeBits)) & TickEnums::NodeMask;
    }
	else if (timeLeft < ((uint64_t)1 << (TickEnums::RootBits + TickEnums::NodeBits * 3))) {
        node = 3;
        slot = (expires >> (TickEnums::RootBits + TickEnums::NodeBits * 2)) & TickEnums::NodeMask;
    }
	else if (timeLeft < ((uint64_t)1 << (TickEnums::RootBits + TickEnums::NodeBits * 4))) {
        node = 4;
        slot = (expires >> (TickEnums::RootBits + TickEnums::NodeBits * 3)) & TickEnums::NodeMask;
    }
	else {
		f_assert(false);
	}

    nodes[node]->slots[slot].push_back(context);
}

unsigned int
tickMgr::genTickId() {
	static unsigned int serial_number = 1;
    unsigned int tickId = serial_number++;
	while (this->getContext(tickId) || tickId == InvalidTickId) {
		tickId = serial_number++;
	}

	return tickId;
}

void 
tickMgr::removeContext(unsigned int id) {
    _timer_context *context = this->getContext(id);
    if (context == nullptr) {
		Log::Error("[tickMgr::removeContext][tickId: 0xu]: not found", id);
        return;
    }
    this->removeContext(context);
}

void 
tickMgr::removeContext(_timer_context *context) {
	this->contexts.Set(context->id, nullptr);
    Memory::Delete(context);
}


tickMgr::_timer_context*
tickMgr::getContext(unsigned int id) {
    return this->contexts.Get(id);
}


unsigned int
tickMgr::getNodeSlot(unsigned char nodeId) {
    return this->jiffies >> (TickEnums::RootBits + (nodeId - 1) * TickEnums::NodeBits) & TickEnums::NodeMask;
}

void
tickMgr::cascadeTimers(unsigned char nodeId) {
    unsigned int slot = this->getNodeSlot(nodeId);
    _timer_node *node = this->nodes[nodeId];

    auto &lst = node->slots[slot];
    while (!lst.empty()) {
		_timer_context *context = lst.front();
        this->addContext(context);
		lst.pop_front();
    }
}

}
}