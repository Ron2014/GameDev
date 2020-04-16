#pragma once
#include "Core/3D/Vector3D.h"
#include "TerrainGrid.h"
#include <string>
#include <vector>

class TerrainConfig
{
public:
    TerrainConfig();
    ~TerrainConfig();

    void LoadData(int id);
    void LoadData(std::string filename);
    void SaveData();
    void DumpData();

    int Pos2GridRow(Vector3D pos);
    int Pos2GridCol(Vector3D pos);

    grid_type GetPointType(Vector3D pos);
    grid_type GetGridType(int col, int row);

    int GetGridRow();
    int GetGridCol();

private:
    const int VERSION = 10;
    const char* MAP_FILE_PATH = "E:/GitHub/GameDev/resource/maps";

    std::string _sName;
    std::string _sStage;
    std::string _sLevel;

    double _dLineWidth;
    double _dLineLength;
    int _iLinePix;

    int _iGridRow;
    int _iGridCol;

    Vector3D _vCenter;

    std::vector<std::vector<GridInfo> > _gridInfos;
};