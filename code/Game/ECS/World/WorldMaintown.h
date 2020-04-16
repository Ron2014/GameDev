#pragma once
#include "World.h"

class WorldMaintown: public World
{
private:
    /* data */
public:
    WorldMaintown(/* args */);
    ~WorldMaintown();
};

WorldMaintown::WorldMaintown(/* args */):World()
{
    SetType(World::maintown);
}

WorldMaintown::~WorldMaintown()
{
}
