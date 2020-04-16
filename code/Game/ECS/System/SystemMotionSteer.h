#pragma once
#include "System.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentMoving.h"

/**
 * entity & component
 * ComponentLocation
 * ComponentMoving
*/

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

void SystemMotionSteer::Update() {
    Entity *e = gEntityMgr.GetMember(m_iPlayerID);
    if (e) {
        // signal from keyboard
        // signal from joystick(xbox controller)
    } else {
        Log::Error("Render Error: entity %d not exist!", m_iPlayerID);
    }
}