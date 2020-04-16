#pragma once

#include "Component.h"

struct ComponentHealthPoint : public Component
{
    ComponentHealthPoint():Component() {
        SetType(Component::health_point);
    }

    int iMaxHP;
    int iCurHP;
};
