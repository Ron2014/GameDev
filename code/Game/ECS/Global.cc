#pragma once
#include "Global.h"

bool gExitGame = false;

ObjectManager<World> gWorldMgr;
ObjectManager<Entity> gEntityMgr;

int gWatchingWorldID = 0;
bool gRefreshWorld = false;