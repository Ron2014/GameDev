#pragma once

#include <functional>
#include <list>
#include <set>

#include "Enums.h"
#include "Core/Util/ObjectContainer.h"


namespace Future {


const static unsigned int InvalidTickId = 0;

namespace _priv {

class tickMgr {
    typedef std::function<void()> TickFun;
public:
	tickMgr() :contexts("tickMgr") {}
    /// setup the wrapper
    void Setup();
    /// discard the wrapper
    void Discard();
    /// return true if wrapper is valid
    bool IsValid() const;
    
    unsigned int RegisterTick(uint64_t interval, TickFun func, const char* tickName);
	void UnregisterTick(unsigned int id);

    void PushTicks();

    unsigned int GetTickCount();

private:

    struct _timer_context {
		unsigned int id;
        uint64_t interval;
        uint64_t expires;
        TickFun func;
		std::string name;
        bool terminate = false;
    };

    struct _timer_root {
        std::list<_timer_context*> slots[TickEnums::RootSize];
    };

    struct _timer_node {
		std::list<_timer_context*> slots[TickEnums::NodeSize];
    };

    _timer_root root;
    _timer_node node2;
    _timer_node node3;
    _timer_node node4;
    _timer_node node5;

	_timer_node* nodes[TickEnums::TotalNode];

private:
    unsigned int genTickId();
	void removeContext(unsigned int id);
    void removeContext(_timer_context *context);

    void addContext(_timer_context *context);
	_timer_context* getContext(unsigned int id);

    void cascadeTimers(unsigned char nodeId);
    unsigned int getNodeSlot(unsigned char nodeId);

    uint64_t lapTime();

private:
    bool isValid = false;

    uint64_t jiffies = 0;
    uint64_t lastMillisec;

	ObjectContainer<_timer_context> contexts;
	std::set<unsigned int> dirty_contexts;
};


}
}