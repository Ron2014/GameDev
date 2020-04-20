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
extern int gWatchingSceneID;
extern bool gRefreshScene;