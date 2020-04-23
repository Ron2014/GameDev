#pragma once
#include "Core/Object/ObjectManager.h"
#include "Terrain/TerrainConfig.h"
#include "Entity/Entity.h"
#include "World/World.h"

// running switch
extern bool gExitGame;

// all world & entity
extern ObjectManager<std::string, TerrainConfig> gTerrainConfigMgr;
extern ObjectManager<int, World> gWorldMgr;
extern ObjectManager<int, Entity> gEntityMgr;

// has any moving entity, need refresh scene
extern int gWatchingWorldID;
extern bool gRefreshWorld;

// has an entity to control, need refresh control window
extern int gContollingEntityID;
extern bool gRefreshControl;