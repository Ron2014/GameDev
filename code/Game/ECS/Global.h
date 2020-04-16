#pragma once
#include "Core/Object/ObjectManager.h"
#include "Entity/Entity.h"
#include "World/World.h"

// all world & entity
static ObjectManager<World> gWorldMgr;
static ObjectManager<Entity> gEntityMgr;