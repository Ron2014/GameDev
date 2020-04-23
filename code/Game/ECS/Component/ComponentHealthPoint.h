#pragma once

#include "Component.h"

struct ComponentHealthPoint : public Component
{
public:
    static const TYPE type = Component::health_point;
    
    int iMaxHP;
    int iCurHP;
};