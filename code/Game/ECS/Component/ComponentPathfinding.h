#pragma once
#include "Component.h"
#include "Core/3D/Vector3D.h"
#include "System/SystemPathfinding.h"
#include <set>

typedef struct PathNode{
    Vector3D point;
    PathNode *next;
} PathNode;

struct ComponentPathfinding: public Component
{
public:
    ComponentPathfinding():Component(){
        SetType(Component::path_finding);
    }

    PathNode *head;

    virtual void AddTo(Entity *e) {
        SystemPathfinding::Instance()->RegisterEntity(e->GetID());
    }

    virtual void RemoveFrom(Entity *e) {
        SystemPathfinding::Instance()->UnregisterEntity(e->GetID());
    }
};
