#include "SystemPathfinding.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentMoving.h"
#include "Component/ComponentPathfinding.h"

/**
 * entity & component
 * ComponentLocation
 * ComponentMoving
 * ComponentPathfinding
*/

SystemPathfinding::SystemPathfinding(/* args */):System()
{
    SetType(System::path_finding);
}

SystemPathfinding::~SystemPathfinding()
{
    m_sEntityIDs.clear();
}

void SystemPathfinding::RegisterEntity(int entity_id) {
    m_sEntityIDs.insert(entity_id);

    // now, let's make a plan to get the target
    Entity *e = gEntityMgr.GetMember(entity_id);
    Component *c = e->GetComponent(Component::location);
    Vector3D position = ((ComponentLocation *)c)->vPosition;
    c = e->GetComponent(Component::path_finding);
    Vector3D destination = ((ComponentPathfinding *)c)->head->point;

    // TODO: how to arrive destination from position?
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
            Vector3D &position = ((ComponentLocation *)c)->vPosition;
            Vector3D &heading = ((ComponentLocation *)c)->vHeading;

            c = e->GetComponent(Component::path_finding);

            // variable for pathfinding
            PathNode *node = ((ComponentPathfinding *)c)->head;
            Vector3D &nextPos = node->point;

            double distance = nextPos.Distance(position);
            if (Math::isEqual(distance, 0.0)) {
                // arrive this node;
                free(node);
                node = node->next;
                ((ComponentPathfinding *)c)->head = node;

                if (node) {
                    // next target
                    nextPos = node->point;
                    c = e->GetComponent(Component::moving);

                    Vector3D &velocity = ((ComponentMoving *)c)->vVelocity;
                    velocity = nextPos - position;
                    int maxSpeed = ((ComponentMoving *)c)->iMaxSpeed;
                    velocity.Truncate(maxSpeed);

                } else {
                    // finish pathfinding
                    e->RemoveComponent(Component::path_finding);
                }
            }

        } else {
            Log::Error("Render Error: entity %d not exist!", *it);
        }
    }
}