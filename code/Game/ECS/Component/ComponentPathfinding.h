#pragma once
#include "Component.h"
#include "Core/3D/Vector3D.h"
using namespace Future;

typedef struct PathNode{
    Vector3D point;
    PathNode *next;
} PathNode;

struct ComponentPathfinding: public Component
{
public:
    PathNode *head;
    
    ComponentPathfinding();
    ~ComponentPathfinding();

    void SetDestination(const Vector3D &destination);
    virtual void AddTo(Entity *);
    virtual void RemoveFrom(Entity *);
};
