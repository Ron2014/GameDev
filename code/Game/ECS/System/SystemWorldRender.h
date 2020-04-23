#pragma once
#include "System.h"
#include "Terrain/TerrainConfig.h"

class SystemWorldRender: public System
{
protected:
    // which world are you wathing
    int m_iWorldID;
    static int refresh_count;

    SystemWorldRender();
    ~SystemWorldRender();

    void DrawTerrain(TerrainConfig *terrainCfg);
    void DrawEntity(TerrainConfig *terrainCfg, std::set<int> &entity_ids);

public:
    static const System::TYPE type = System::world_render;

    void SetWorldID(int worldID) { 
        m_iWorldID = worldID;
        gWatchingWorldID = worldID;
        gRefreshWorld = true;
    }
    int GetWorldID() { return m_iWorldID; }

    virtual void Update();
    SINGLETON(SystemWorldRender)
};