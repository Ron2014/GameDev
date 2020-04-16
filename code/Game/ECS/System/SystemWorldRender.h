#pragma once
#include "System.h"

class SystemWorldRender: public System
{
protected:
    // which world are you wathing
    int m_iWorldID;

    SystemWorldRender():m_iWorldID(0),System() {
        SetType(System::world_render);
    }

public:
    void SetWorldID(int worldID) { m_iWorldID = worldID; }
    int GetWorldID() { return m_iWorldID; }

    virtual void Update();
    SINGLETON(SystemWorldRender)
};