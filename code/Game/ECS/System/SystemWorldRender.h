#pragma once
#include "System.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentHealthPoint.h"

/**
 * world -> entity & component
 * ComponentLocation
 * ComponentHealthPoint
*/

class SystemWorldRender: public System
{
protected:
    // which world are you wathing
    int m_iWorldID;

    SystemWorldRender():m_iWorldID(0),System() {
        SetType(System::render);
    }

public:
    void SetWorldID(int worldID) { m_iWorldID = worldID; }
    int GetWorldID() { return m_iWorldID; }

    virtual void Update();
    SINGLETON(SystemWorldRender)
};

void SystemWorldRender::Update()
{
    // which world need render
    World *world = gWorldMgr.GetMember(m_iWorldID);
    if (!world){
        Log::Error("Render Error: world %d not exist!", m_iWorldID);
        return;
    }

    // render terrain of this world
    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    terrainCfg->DumpData();

    // get entity from world
    std::set<int> entity_ids = world->GetEntityIDs();
    std::set<int>::iterator it = entity_ids.begin();

    while( it != entity_ids.end()) {
        // entity exist ?
        Entity *e = gEntityMgr.GetMember(*it);

        if (e) {
            // get ComponentLocation
            Component *c = e->GetComponent(Component::location);

            if (c) {
                // get position & heading
                Vector3D position = ((ComponentLocation *)c)->vPosition;
                Vector3D heading = ((ComponentLocation *)c)->vHeading;

                // render

                c = e->GetComponent(Component::health_point);
                if (c) {
                    int maxHP = ((ComponentHealthPoint *)c)->iMaxHP;
                    int curHP = ((ComponentHealthPoint *)c)->iCurHP;

                    // render
                }
            }
        } else {
            Log::Error("Render Error: entity %d not exist!", *it);
        }

        it++;
    }
}