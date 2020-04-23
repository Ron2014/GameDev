#include "ComponentMoving.h"
#include "System/SystemMoving.h"

// Component::TYPE ComponentMoving::type = Component::moving;

void ComponentMoving::AddTo(Entity *e) {
    SystemMoving::Instance()->RegisterEntity(e->GetID());
}

void ComponentMoving::RemoveFrom(Entity *e) {
    SystemMoving::Instance()->UnregisterEntity(e->GetID());
}