#pragma once
#include "Global.h"

bool gExitGame = false;

ObjectManager<std::string, TerrainConfig> gTerrainConfigMgr;
ObjectManager<int, World> gWorldMgr;
ObjectManager<int, Entity> gEntityMgr;

int gWatchingWorldID = 0;
bool gRefreshWorld = false;

int gContollingEntityID = 0;
bool gRefreshControl = false;