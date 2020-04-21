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

    int Pos2GridRow(const Vector3D &pos);
    int Pos2GridCol(const Vector3D &pos);
    void WrapAround(Vector3D &pos);

    grid_type GetPointType(const Vector3D &pos);
    grid_type GetGridType(int col, int row);

    int GetGridRow();
    int GetGridCol();
    int GetLinePixel();
    double GetLineLength();

private:
    const int VERSION = 10;
    static char* MAP_FILE_PATH;

    std::string _sName;
    std::string _sStage;
    std::string _sLevel;

    double _dLineWidth;             // the thickness of grid line
    double _dLineLength;            // the length of grid line
    int _iLinePix;                  // count of pixel for a grid line

    int _iGridRow;
    int _iGridCol;

    Vector3D _vCenter;

    std::vector<std::vector<GridInfo> > _gridInfos;
};