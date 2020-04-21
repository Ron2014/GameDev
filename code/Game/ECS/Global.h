#pragma once
#include "Core/Object/ObjectManager.h"
#include "Entity/Entity.h"
#include "World/World.h"

// running switch
extern bool gExitGame;

// all world & entity
extern ObjectManager<World> gWorldMgr;
extern ObjectManager<Entity> gEntityMgr;

// has any moving entity, need refresh scene
extern int gWatchingWorldID;
extern bool gRefreshWorld;

// has an entity to control, need refresh control window
extern int gContollingEntityID;
extern bool gRefreshControl;