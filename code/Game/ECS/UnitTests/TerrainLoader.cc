#include <iostream>
#include "UnitTest++/src/UnitTest++.h"
#include "Terrain/TerrainConfig.h"

using namespace Future;

TEST(TerrainLoader) {
    for(int i = 0; i < TerrainGrid::TypeCount; i++) {
        std::cout << TerrainGrid::type_chtype[i] << std::endl;
    }
    std::cout << ACS_CKBOARD << std::endl;
    std::string filename;
    std::cout << "input the filename: ";
    std::cin >> filename;

    TerrainConfig tc = TerrainConfig();
    tc.LoadData(filename);
    tc.DumpData();
}