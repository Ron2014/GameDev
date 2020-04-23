#pragma once
#include "Core/3D/Vector3D.h"
#include "Component.h"
using namespace Future;

struct ComponentLocation : public Component
{
public:
    static const TYPE type = Component::location;

    int iWorldID;           // id for world
    Vector3D vPosition;     // vector 3d for position
    Vector3D vHeading;      // vector 3d for heading
};
