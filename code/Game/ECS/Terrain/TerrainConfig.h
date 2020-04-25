#pragma once
#include "Core/3D/Vector3D.h"
#include "TerrainGrid.h"
#include <string>
#include <vector>

class TerrainConfig
{
public:
    static char* MAP_FILE_PATH;
    
    TerrainConfig();
    ~TerrainConfig();

    void LoadData(int id);
    void LoadData(std::string filename);
    void SaveData();
    void DumpData();

    void WrapAround(Vector3D &pos);

    TerrainGrid::TYPE GetPointType(const Vector3D &pos);
    TerrainGrid::TYPE GetGridType(int col, int row);
    const Vector3D &GetGridCenterPos(int col, int row);

    void SetPointType(const Vector3D &, TerrainGrid::TYPE);
    void SetGridType(int, int, TerrainGrid::TYPE);

    std::string GetName();
    int GetGridRow();
    int GetGridCol();
    int GetLinePixel();
    double GetLineLength();

private:
    const int VERSION = 10;

    std::string _sName;
    std::string _sStage;
    std::string _sLevel;

    double _dLineWidth;             // the thickness of grid line
    double _dLineLength;            // the length of grid line
    int _iLinePix;                  // count of pixel for a grid line

    int _iGridRow;
    int _iGridCol;

    Vector3D _vCenter;

    std::vector<std::vector<TerrainGrid> > _gridInfos;
};