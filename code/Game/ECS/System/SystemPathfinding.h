#pragma once
#include <set>
#include "System.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentMoving.h"
#include "Component/ComponentPathfinding.h"

/**
 * entity & component
 * ComponentLocation
 * ComponentMoving
 * ComponentPathfinding
*/

class SystemPathfinding: public System
{
protected:
    std::set<int> m_sEntityIDs;
    SystemPathfinding();

public:
    void RegisterEntity(int);
    void UnregisterEntity(int);

    virtual void Update();
    SINGLETON(SystemPathfinding)
};

SystemPathfinding::SystemPathfinding(/* args */):System()
{
    SetType(System::path_finding);
}

void SystemPathfinding::RegisterEntity(int entity_id) {
    m_sEntityIDs.insert(entity_id);
}

void SystemPathfinding::UnregisterEntity(int entity_id) {
    m_sEntityIDs.erase(entity_id);
}

void SystemPathfinding::Update() {
    std::set<int>::iterator it = m_sEntityIDs.begin();

    while( it != m_sEntityIDs.end()) {
        // entity exist ?
        Entity *e = gEntityMgr.GetMember(*it);

        if (e) {
            // get ComponentLocation
            Component *c = e->GetComponent(Component::location);

            // get position & heading
            Vector3D position = ((ComponentLocation *)c)->vPosition;
            Vector3D heading = ((ComponentLocation *)c)->vHeading;

            c = e->GetComponent(Component::moving);

            // variable for moving
            Vector3D velocity = ((ComponentMoving *)c)->vVelocity;
            int mass = ((ComponentMoving *)c)->iMass;
            int maxSpeed = ((ComponentMoving *)c)->iMaxSpeed;
            int maxForce = ((ComponentMoving *)c)->iMaxForce;
            int maxTurnRate = ((ComponentMoving *)c)->iMaxTurnRate;

            c = e->GetComponent(Component::path_finding);

            // variable for pathfinding
            PathNode *node = ((ComponentPathfinding *)c)->head;
            Vector3D nextPos = node->point;

            // calculate new pos

        } else {
            Log::Error("Render Error: entity %d not exist!", *it);
        }
}