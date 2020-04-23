#pragma once
#include "Component.h"
#include "Core/3D/Vector3D.h"
using namespace Future;

struct ComponentMoving: public Component
{
public:
    static const TYPE type = Component::moving;
    
    Vector3D vVelocity;
    int iMass;
    int iMaxSpeed;
    int iMaxForce;
    int iMaxTurnRate;
    
    virtual void AddTo(Entity *);
    virtual void RemoveFrom(Entity *);
};
