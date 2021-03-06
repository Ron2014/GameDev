#include "World.h"
#include "Global.h"

int World::m_iNextValidID = 0;

const std::string World::world_names[World::TypeCount] = {
    "none",
    "maintown",
    "battle",
    "terrain_editor",
};

World::World() {
    SetID(m_iNextValidID);
    m_iNextValidID++;
    if (m_iNextValidID < 0)
        m_iNextValidID = 0;

    gWorldMgr.AddMember(GetID(), this);
}

World::~World() {
    m_sEntityIDs.clear();
}

void World::Enter(Entity *e) {
    m_sEntityIDs.insert(e->GetID());
}

void World::Leave(Entity *e) {
    m_sEntityIDs.erase(e->GetID());
}

void World::LoadTerrain(const std::string &filename) {
    TerrainConfig *terrainCfg = gTerrainConfigMgr.GetMember(filename);
    if (!terrainCfg) {
        terrainCfg = new TerrainConfig();
        gTerrainConfigMgr.AddMember(filename, terrainCfg);
    }
    terrainCfg->LoadData(filename + TerrainConfig::FILE_EXTENSION);
    m_sTerrainName = filename;
}

TerrainConfig *World::LoadTempTerrain(const std::string &filename) {
    TerrainConfig *terrainCfg = gTerrainConfigMgr.GetMember(filename);
    if (!terrainCfg) {
        terrainCfg = new TerrainConfig();
        gTerrainConfigMgr.AddMember(filename, terrainCfg);
    }
    m_sTerrainName = filename;
    return terrainCfg;
}

TerrainConfig *World::GetTerrainConfig() {
    return gTerrainConfigMgr.GetMember(m_sTerrainName);
}
