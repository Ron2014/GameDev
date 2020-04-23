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
}

SystemPathfinding::~SystemPathfinding()
{
    m_sEntityIDs.clear();
}

void SystemPathfinding::RegisterEntity(int entity_id) {
    m_sEntityIDs.insert(entity_id);

    // now, let's make a plan to get the target
    Entity *e = gEntityMgr.GetMember(entity_id);
    ComponentLocation *cl = e->GetComponent<ComponentLocation>();
    Vector3D position = cl->vPosition;
    ComponentPathfinding *cp = e->GetComponent<ComponentPathfinding>();
    Vector3D destination = cp->head->point;

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
            // get position & heading
            ComponentLocation *cl = e->GetComponent<ComponentLocation>();
            Vector3D &position = cl->vPosition;
            Vector3D &heading = cl->vHeading;

            // variable for pathfinding
            ComponentPathfinding *cp = e->GetComponent<ComponentPathfinding>();
            PathNode *node = cp->head;
            Vector3D &nextPos = node->point;

            double distance = nextPos.Distance(position);
            if (Math::isEqual(distance, 0.0)) {
                // arrive this node;
                free(node);
                node = node->next;
                cp->head = node;

                if (node) {
                    // next target
                    nextPos = node->point;
                    ComponentMoving *cm = e->GetComponent<ComponentMoving>();
                    Vector3D &velocity = cm->vVelocity;
                    velocity = nextPos - position;
                    int maxSpeed = cm->iMaxSpeed;
                    velocity.Truncate(maxSpeed);

                } else {
                    // finish pathfinding
                    e->RemoveComponent<ComponentPathfinding>();
                }
            }

        } else {
            Log::Error("Render Error: entity %d not exist!", *it);
        }
    }
}