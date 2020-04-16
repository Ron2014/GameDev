#pragma once
#include "Core/Object/ObjectManager.h"
#include "Entity/Entity.h"
#include "World/World.h"

// all world & entity
extern ObjectManager<World> gWorldMgr;
extern ObjectManager<Entity> gEntityMgr;