#pragma once
#include "Core/3D/Vector3D.h"
#include <curses.h>
using namespace Future;

chtype grid_chtype[] = {
    0, ACS_CKBOARD, ACS_DIAMOND, ACS_LANTERN, ACS_PLUS, ACS_PLMINUS,
};

struct TerrainGrid {
    enum Type {
        Walkable,
        Walkable_Non,
        StartPoint,
        Flyable,
        EndPoint,
        HeroPoint,
        TypeCount,
    };

    Type gridType;
    Vector3D centerPos;

    TerrainGrid(Type type, Vector3D v3):
        gridType(type),
        centerPos(v3){
            
    }

    TerrainGrid():gridType(Walkable_Non) {
    }
};