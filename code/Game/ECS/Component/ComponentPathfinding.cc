#include "ComponentPathfinding.h"
#include "System/SystemPathfinding.h"

void ComponentPathfinding::AddTo(Entity *e) {
    SystemPathfinding::Instance()->RegisterEntity(e->GetID());
}

void ComponentPathfinding::RemoveFrom(Entity *e) {
    SystemPathfinding::Instance()->UnregisterEntity(e->GetID());
}