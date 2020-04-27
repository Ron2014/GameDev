#include "TerrainGrid.h"

chtype TerrainGrid::type_chtype[TerrainGrid::TypeCount] = {0,0,0};

TerrainGrid::TerrainGrid(TYPE type, Vector3D v3):
    gridType(type),
    centerPos(v3){
        
}

TerrainGrid::TerrainGrid():
    gridType(None),
    centerPos(Vector3D()) {
}