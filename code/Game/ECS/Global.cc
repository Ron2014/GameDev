#pragma once
#include "Global.h"

bool gExitGame = false;

ObjectManager<World> gWorldMgr;
ObjectManager<Entity> gEntityMgr;

int gWatchingSceneID = 0;
bool gRefreshScene = false;