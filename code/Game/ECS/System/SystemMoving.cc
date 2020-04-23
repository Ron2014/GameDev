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
            // get position & heading
            ComponentLocation *cl = e->GetComponent<ComponentLocation>();
            Vector3D &position = cl->vPosition;
            Vector3D &heading = cl->vHeading;
            int world_id = cl->iWorldID;

            // variable for moving
            ComponentMoving *cv = e->GetComponent<ComponentMoving>();
            Vector3D &velocity = cv->vVelocity;
            
            if (!velocity.isZero()) {
                if (world_id == gWatchingWorldID) gRefreshWorld = true;
                if (e->GetID() == gContollingEntityID ) gRefreshControl = true;
                
                World *world = gWorldMgr.GetMember(world_id);
                TerrainConfig *terrainCfg = world->GetTerrainConfig();

                // assume 1 second passed
                double passedTime = 1.0;
                position += velocity * passedTime;

                // collision check
                
                // bounds check
                terrainCfg->WrapAround(position);
            }

        } else {
            Log::Error("Render Error: entity %d not exist!", *it);
        }

        it++;
    }
}