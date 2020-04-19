#include <iostream>
#include <fstream>
#include <cassert>
#include "TerrainConfig.h"
#include "Core/Log/Log.h"
#include "Core/IO/Path.h"

using namespace Future;

#if FUTURE_WINDOWS
char * TerrainConfig::MAP_FILE_PATH = "E:/GitHub/GameDev/resource/maps";
#else
char * TerrainConfig::MAP_FILE_PATH = "/mnt/e/GitHub/GameDev/resource/maps";
#endif

TerrainConfig::TerrainConfig():
    _dLineWidth(0.05),
    _dLineLength(1.0),
    _iLinePix(1){
}

TerrainConfig::~TerrainConfig() {
    _gridInfos.clear();
}

int TerrainConfig::GetGridRow() {
    return _iGridRow;
}

int TerrainConfig::GetGridCol() {
    return _iGridCol;
}

void TerrainConfig::LoadData(int id) {
    // get filename from id
}

void TerrainConfig::LoadData(std::string filename) {
    std::string fullpath = Path::Combine(MAP_FILE_PATH, filename.c_str());
    if (!Path::FileExists(fullpath))
        Log::Error("file %s not exist!", fullpath);

    std::ifstream fin(fullpath, std::ios::binary);
    
    unsigned char version;
    fin.read((char *)&version, sizeof(unsigned char));

    if(VERSION!=version){
        Log::Error("Terrain(%s) Version(%d) not matched!", filename, version);
        return;
    }
    
    char tmp[255];
    unsigned char length;
    fin.read((char*)&length, sizeof(unsigned char));
    fin.read(tmp, length);
    tmp[length] = '\0';
    _sName = tmp;

    fin.read((char*)&length, sizeof(unsigned char));
    fin.read(tmp, length);
    tmp[length] = '\0';
    _sStage = tmp;

    fin.read((char*)&length, sizeof(unsigned char));
    fin.read(tmp, length);
    tmp[length] = '\0';
    _sLevel = tmp;

    if(Path::GetBaseFilename(filename) != _sName)
        Log::Error("Terrain(%s) Name(%s) not matched!", filename.c_str(), _sName.c_str());
    
    int lineWidth, lineLength;
    fin.read((char *)&lineWidth, sizeof(int));
    fin.read((char *)&lineLength, sizeof(int));

    _dLineWidth = lineWidth * 0.001;
    _dLineLength = lineLength * 0.001;

    fin.read((char*)&_iLinePix, sizeof(int));

    int x, y, z;
    fin.read((char*)&x, sizeof(int));
    fin.read((char*)&y, sizeof(int));
    fin.read((char*)&z, sizeof(int));
    _vCenter.x = (double)x;
    _vCenter.y = (double)y;
    _vCenter.z = (double)z;

    fin.read((char*)&_iGridCol, sizeof(int));
    fin.read((char*)&_iGridRow, sizeof(int));

    unsigned char gridType;
    _gridInfos.resize(_iGridRow);
    for (int i=0; i<_iGridRow; i++) {
        _gridInfos[i].resize(_iGridCol);
        for (int j=0; j<_iGridCol; j++) {
            fin.read((char*)&gridType, sizeof(unsigned char));
            _gridInfos[i][j].gridType = (grid_type)gridType;

            fin.read((char*)&x, sizeof(int));
            fin.read((char*)&y, sizeof(int));
            fin.read((char*)&z, sizeof(int));
            _gridInfos[i][j].centerPos.x = x * 0.01;
            _gridInfos[i][j].centerPos.y = y * 0.01;
            _gridInfos[i][j].centerPos.z = z * 0.01;
        }
    }

    fin.close();
}

void TerrainConfig::SaveData() {
    std::string filename = Path::Combine(MAP_FILE_PATH, _sName.c_str());
    std::ofstream fout(filename, std::ios::binary);
    fout.write((char*)&VERSION, sizeof(unsigned char));

    unsigned char length = (unsigned char)_sName.length();
    fout.write((char *)&length, sizeof(unsigned char));
    fout.write(_sName.c_str(), length);

    length = (unsigned char)_sName.length();
    fout.write((char *)&length, sizeof(unsigned char));
    fout.write(_sStage.c_str(), length);

    length = (unsigned char)_sName.length();
    fout.write((char *)&length, sizeof(unsigned char));
    fout.write(_sLevel.c_str(), length);

    int lineWidth = int(_dLineWidth * 1000);
    int lineLength = int(_dLineLength * 1000);
    fout.write((char*)&lineWidth, sizeof(int));
    fout.write((char*)&lineLength, sizeof(int));
    fout.write((char*)&_iLinePix, sizeof(int));

    int x = (int)_vCenter.x;
    int y = (int)_vCenter.y;
    int z = (int)_vCenter.z;
    fout.write((char*)&x, sizeof(int));
    fout.write((char*)&y, sizeof(int));
    fout.write((char*)&z, sizeof(int));

    fout.write((char*)&_iGridCol, sizeof(int));
    fout.write((char*)&_iGridRow, sizeof(int));

    unsigned char gridType;
    for (int i=0; i<_iGridRow; i++) {
        for (int j=0; j<_iGridCol; j++) {
            gridType = (unsigned char)_gridInfos[i][j].gridType;
            fout.write((char*)&gridType, sizeof(unsigned char));
            x = int(_gridInfos[i][j].centerPos.x * 100);
            y = int(_gridInfos[i][j].centerPos.y * 100);
            z = int(_gridInfos[i][j].centerPos.z * 100);
            fout.write((char*)&x, sizeof(int));
            fout.write((char*)&z, sizeof(int));
            fout.write((char*)&y, sizeof(int));
        }
    }

    fout.close();
}

void TerrainConfig::DumpData() {
    std::cout << "Name: " << _sName << std::endl;
    std::cout << "Stage: " << _sStage << std::endl;
    std::cout << "Level: " << _sLevel << std::endl;

    printf("lineWidth: %0.3f\nlineLength: %0.3f\nlinePix: %d\n", _dLineWidth, _dLineLength, _iLinePix);
    printf("center: %0.3f %0.3f %0.3f\n", _vCenter.x, _vCenter.y, _vCenter.z);

    for (int i=0; i<_iGridRow; i++) {
        for (int j=0; j<_iGridCol; j++) {
            printf(" %d |", _gridInfos[_iGridRow-i-1][j].gridType);
        }
        printf("\n");
    }
}

int TerrainConfig::Pos2GridRow(const Vector3D &pos) {
    return int(pos.z / _dLineLength);
}

int TerrainConfig::Pos2GridCol(const Vector3D &pos) {
    return int(pos.x / _dLineLength);
}

void TerrainConfig::WrapAround(Vector3D &pos) {
    // map size
    double map_width = _dLineLength * _iGridCol;
    double map_height = _dLineLength * _iGridRow;

    if (pos.x < 0) pos.x += map_width;
    if (pos.z < 0) pos.z += map_height;
    
    int xtimes = int(pos.x / map_width);
    int ztimes = int(pos.z / map_height);
    pos.x -= xtimes * map_width;
    pos.z -= ztimes * map_height;
}

grid_type TerrainConfig::GetPointType(const Vector3D &pos) {
    int col = int(pos.x / _dLineLength);
    int row = int(pos.z / _dLineLength);
    return GetGridType(col, row);
}

grid_type TerrainConfig::GetGridType(int col, int row) {
    if (col < 0 || col >= _iGridCol || row < 0 || row >= _iGridRow) // use [at] for out_of_range exception
        Log::Error("grid pos(%d,%d) out of bound(%d,%d)", col, row, _iGridCol, _iGridRow);
    return _gridInfos[row][col].gridType;
}