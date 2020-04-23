#pragma once
#include "World.h"

class WorldBattle: public World
{
private:
    /* data */
public:
    static const World::TYPE type = World::battle;
    WorldBattle();
    ~WorldBattle();
};

WorldBattle::WorldBattle(/* args */):World()
{
}

WorldBattle::~WorldBattle()
{
}
