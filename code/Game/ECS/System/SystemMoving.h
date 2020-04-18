#pragma once
#include <set>
#include "System.h"

class SystemMoving: public System
{
protected:
    std::set<int> m_sEntityIDs;
    SystemMoving();

public:
    ~SystemMoving();
    void RegisterEntity(int);
    void UnregisterEntity(int);

    virtual void Update();
    SINGLETON(SystemMoving)
};