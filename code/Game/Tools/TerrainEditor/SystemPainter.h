#pragma once
#include "ECS/System/System.h"

class SystemPainter : System
{
protected:
    // which player are you controlling
    int m_iPlayerID;

    SystemPainter();

public:
    void SetPlayerID(int playerID) {
        m_iPlayerID = playerID;
        gContollingEntityID = playerID;
        gRefreshControl = true;
    }
    int GetPlayerID() { return m_iPlayerID; }
    
    virtual void Update();
    SINGLETON(SystemPainter)
};