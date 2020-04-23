#pragma once
#include "ECS/Component/Component.h"
#include "Terrain/TerrainGrid.h"

struct ComponentPainter: Component
{
public:
    ComponentPainter():Component() {
        SetType(Component::painter);
    }
    TerrainGrid::TYPE pen;
};
