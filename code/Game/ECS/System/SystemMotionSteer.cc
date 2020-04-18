#include "SystemMotionSteer.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentMoving.h"

using namespace Future;

/**
 * entity & component
 * ComponentLocation
 * ComponentMoving
*/

#if FUTURE_WINDOWS

void SystemMotionSteer::Update() {
    Entity *e = gEntityMgr.GetMember(m_iPlayerID);
    if (e) {
        // signal from keyboard
        // signal from joystick(xbox controller)
    } else {
        Log::Error("Render Error: entity %d not exist!", m_iPlayerID);
    }
}

#else

#include <curses.h>

void SystemMotionSteer::Update() {
    int c;
    while ((c = getch()) != ERR)
        m_sHoldingKeys.insert(c);

    Entity *e = gEntityMgr.GetMember(m_iPlayerID);
    if (e) {
        // signal from keyboard
        // signal from joystick(xbox controller)

        ComponentMoving *c = (ComponentMoving *)e->GetComponent(Component::moving);

        if (m_sHoldingKeys.count('w'))
            c->vVelocity.z += c->iMaxSpeed;
        if (m_sHoldingKeys.count('a'))
            c->vVelocity.x -= c->iMaxSpeed;
        if (m_sHoldingKeys.count('s'))
            c->vVelocity.z -= c->iMaxSpeed;
        if (m_sHoldingKeys.count('d'))
            c->vVelocity.x += c->iMaxSpeed;

        c->vVelocity.Truncate(c->iMaxSpeed);

        if (Math::isEqual(c->vVelocity.z, 0.0)) {
            m_sHoldingKeys.erase('w');
            m_sHoldingKeys.erase('s');
        }

        if (Math::isEqual(c->vVelocity.x, 0.0)) {
            m_sHoldingKeys.erase('a');
            m_sHoldingKeys.erase('d');
        }

    } else {
        Log::Error("Render Error: entity %d not exist!", m_iPlayerID);
    }
}

#endif