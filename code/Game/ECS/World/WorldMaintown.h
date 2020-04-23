#pragma once
#include "World.h"

class WorldMaintown: public World
{
private:
    /* data */
public:
    static const World::TYPE type = World::maintown;
    WorldMaintown(/* args */);
    ~WorldMaintown();
};

WorldMaintown::WorldMaintown(/* args */):World()
{
}

WorldMaintown::~WorldMaintown()
{
}
