#pragma once
#include "System.h"

class SystemMotionSteer: public System
{
protected:
    // which player are you controlling
    int m_iPlayerID;

    SystemMotionSteer():m_iPlayerID(0), System() {
        SetType(System::motion_steer);
    }

public:
    void SetPlayerID(int playerID) { m_iPlayerID = playerID; }
    int GetPlayerID() { return m_iPlayerID; }
    
    virtual void Update();
    SINGLETON(SystemMotionSteer)
};