#pragma once
#include "System.h"

class SystemMotionSteer: public System
{
protected:
    // which player are you controlling
    int m_iPlayerID;

    SystemMotionSteer();

public:
    static const System::TYPE type = System::motion_steer;

    void SetPlayerID(int playerID) {
        m_iPlayerID = playerID;
        gContollingEntityID = playerID;
        gRefreshControl = true;
    }
    int GetPlayerID() { return m_iPlayerID; }
    
    virtual void Update();
    SINGLETON(SystemMotionSteer)
};