#include "WorldTerrainEditor.h"

WorldTerrainEditor::WorldTerrainEditor(/* args */):World()
{
}

WorldTerrainEditor::~WorldTerrainEditor()
{
}

void WorldTerrainEditor::Enter(Entity *entity) {
    m_sEntityIDs.insert(entity->GetID());

    TerrainConfig *terrain = GetTerrainConfig();

    ComponentLocation *c0 = entity->AddComponent<ComponentLocation>();
    c0->iWorldID = GetID();
    c0->vPosition = terrain->GetGridCenterPos(0, 0);
    c0->vHeading.Zero();

    ComponentMoving *c1 = entity->AddComponent<ComponentMoving>();
    c1->vVelocity.Zero();
    c1->iMass = 100;
    c1->iMaxSpeed = terrain->GetLineLength();
    c1->iMaxForce = 1;
    c1->iMaxTurnRate = 0;

    ComponentPainter *c2 = entity->AddComponent<ComponentPainter>();
}