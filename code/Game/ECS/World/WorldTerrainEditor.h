#pragma once
#include "World.h"

class WorldTerrainEditor: public World
{
private:
    /* data */
public:
    static const World::TYPE type = World::terrain_editor;
    WorldTerrainEditor();
    ~WorldTerrainEditor();

    virtual void Enter(Entity *);
};