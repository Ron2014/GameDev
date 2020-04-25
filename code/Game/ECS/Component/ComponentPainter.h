#pragma once
#include "Component/Component.h"
#include "Terrain/TerrainGrid.h"

struct ComponentPainter: Component
{
public:
    static const TYPE type = Component::painter;
    TerrainGrid::TYPE pen;

    ComponentPainter();
};
