#include "SystemMoving.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentMoving.h"
#include "Component/ComponentMoving.h"

/**
 * entity & component
 * ComponentLocation
 * ComponentMoving
 * ComponentMoving
*/

SystemMoving::SystemMoving(/* args */):System()
{
    SetType(System::moving);
}

SystemMoving::~SystemMoving()
{
    m_sEntityIDs.clear();
}

void SystemMoving::RegisterEntity(int entity_id) {
    m_sEntityIDs.insert(entity_id);
}

void SystemMoving::UnregisterEntity(int entity_id) {
    m_sEntityIDs.erase(entity_id);
}

void SystemMoving::Update() {
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

        } else {
            Log::Error("Render Error: entity %d not exist!", *it);
        }
    }
}