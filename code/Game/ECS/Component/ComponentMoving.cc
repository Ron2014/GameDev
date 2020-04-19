#include "ComponentMoving.h"
#include "System/SystemMoving.h"

void ComponentMoving::AddTo(Entity *e) {
    SystemMoving::Instance()->RegisterEntity(e->GetID());
}

void ComponentMoving::RemoveFrom(Entity *e) {
    SystemMoving::Instance()->UnregisterEntity(e->GetID());
}