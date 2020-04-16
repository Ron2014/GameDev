#pragma once
#include <set>
#include "System.h"

class SystemPathfinding: public System
{
protected:
    std::set<int> m_sEntityIDs;
    SystemPathfinding();

public:
    ~SystemPathfinding();
    void RegisterEntity(int);
    void UnregisterEntity(int);

    virtual void Update();
    SINGLETON(SystemPathfinding)
};