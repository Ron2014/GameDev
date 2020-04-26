#include <iostream>
#include "UnitTest++/src/UnitTest++.h"
#include "Terrain/TerrainConfig.h"

using namespace Future;

TEST(TerrainLoader) {
    std::string filename;
    while(true) {
        std::cout << "input the filename: ";
        std::cin >> filename;

        filename += TerrainConfig::FILE_EXTENSION;

        TerrainConfig tc = TerrainConfig();
        tc.LoadData(filename);
        tc.DumpData();
    }
}