#include "SystemMotionSteer.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentMoving.h"

/**
 * entity & component
 * ComponentLocation
 * ComponentMoving
*/

void SystemMotionSteer::Update() {
    Entity *e = gEntityMgr.GetMember(m_iPlayerID);
    if (e) {
        // signal from keyboard
        // signal from joystick(xbox controller)
    } else {
        Log::Error("Render Error: entity %d not exist!", m_iPlayerID);
    }
}