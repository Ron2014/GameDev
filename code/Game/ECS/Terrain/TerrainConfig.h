#pragma once
#include "Core/3D/Vector3D.h"
#include "TerrainGrid.h"
#include <string>
#include <vector>

class TerrainConfig
{
public:
    static char* FILE_PATH;
    static char* DEFAULT_NAME;
    static char* FILE_EXTENSION;
    
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

    std::string GetName() { return _sName; }
    std::string GetStage() { return _sStage; }
    std::string GetLevel() { return _sLevel; }

    int GetGridRow() { return _iGridRow; }
    int GetGridCol() { return _iGridCol; }
    int GetLinePixel() { return _iLinePix; }
    double GetLineLength() { return _dLineLength; }

    void SetName(const char * val) { _sName = val; }
    void SetStage(const char * val) { _sStage = val; }
    void SetLevel(const char * val) { _sLevel = val; }

    void SetGridRow(int row) {
        if (row == _iGridRow) return;

        int oldRow = _iGridRow;
        _iGridRow = row;
        _gridInfos.resize(_iGridRow);
        if (row > oldRow) {
            for (int i=oldRow; i<_iGridRow; i++) {
                _gridInfos[i].resize(_iGridCol);
            }
        }
    }
    void SetGridCol(int col) {
        if (col == _iGridCol) return;
        
        _iGridCol = col;
        for (int i=0; i<_iGridRow; i++) {
            _gridInfos[i].resize(_iGridCol);
        }
    }
    void SetLinePixel(int linePixel) { _iLinePix = linePixel; }
    void SetLineLength(double lineLength) { _dLineLength = lineLength; }

    TerrainConfig &operator=(TerrainConfig &);

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