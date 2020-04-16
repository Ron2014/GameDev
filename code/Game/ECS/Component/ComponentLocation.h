#pragma once
#include "Core/3D/Vector3D.h"
#include "Component.h"
using namespace Future;

struct ComponentLocation : public Component
{
public:
    ComponentLocation():Component() {
        SetType(Component::location);
    }

    // id for world
    int iWorldID;
    // vector 3d for position
    Vector3D vPosition;
    // vector 3d for heading
    Vector3D vHeading;
};