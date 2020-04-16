#pragma once
#include "World.h"

class WorldBattle: public World
{
private:
    /* data */
public:
    WorldBattle();
    ~WorldBattle();
};

WorldBattle::WorldBattle(/* args */):World()
{
    SetType(World::battle);
}

WorldBattle::~WorldBattle()
{
}
