#pragma once
#include "Core/3D/Vector3D.h"
using namespace Future;

enum grid_type {
    Walkable,
    Walkable_Non,
    StartPoint,
    Flyable,
    EndPoint,
    HeroPoint,
};

struct GridInfo {
    grid_type gridType;
    Vector3D centerPos;

    GridInfo(grid_type type, Vector3D v3):
        gridType(type),
        centerPos(v3){
            
    }

    GridInfo():gridType(Walkable_Non) {
    }
};