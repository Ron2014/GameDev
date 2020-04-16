#include <iostream>
#include "UnitTest++/src/UnitTest++.h"
#include "Terrain/TerrainConfig.h"

using namespace Future;

TEST(TerrainLoader) {
    std::string filename;
    std::cout << "input the filename: ";
    std::cin >> filename;

    TerrainConfig tc = TerrainConfig();
    tc.LoadData(filename);
    tc.DumpData();
}