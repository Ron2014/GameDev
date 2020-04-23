#pragma once
#include "Core/3D/Vector3D.h"
#include "Core/Util/NcursesUtil.h"

using namespace Future;

struct TerrainGrid {
public:
    enum TYPE {
        Walkable,
        Walkable_Wall,
        Walkable_Grass,
        TypeCount,
    };

    static chtype type_chtype[TerrainGrid::TypeCount];

    static chtype GetChtype(TYPE type) {
        if (type < 0 || type >= TerrainGrid::TypeCount)
            return 0;
        return type_chtype[type];
    }

    TYPE gridType;
    Vector3D centerPos;

    TerrainGrid(TYPE type, Vector3D v3);
    TerrainGrid();
};