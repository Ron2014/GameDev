#pragma once
#include <set>

#include "Entity/Entity.h"
#include "Component/ComponentLocation.h"
#include "Terrain/TerrainConfig.h"

class World
{
public:
    enum Type {
        maintown,
        battle,       // copy scenes
        TypeCount,
    };

    static const std::string world_names[World::TypeCount];

    static std::string GetName(World::Type type) {
        if (type < 0 || type >= World::TypeCount)
            return "";
        return world_names[type];
    }

protected:
    World::Type m_type;
    int m_ID;
    static int m_iNextValidID;

    // terrain
    TerrainConfig m_terrainConfig;

    // entitys
    std::set<int> m_sEntityIDs;

    World();

    void SetID(int id) { m_ID = id; }
    void SetType(World::Type type) { m_type = type; }

public:
    virtual ~World();

    void LoadTerrain(std::string filename) {
        m_terrainConfig.LoadData(filename);
    }

    TerrainConfig *GetTerrainConfig() {
        return &m_terrainConfig;
    }

    std::set<int>& GetEntityIDs() { return m_sEntityIDs; }
    int GetID() { return m_ID; }
    World::Type GetType() { return m_type; }
    std::string GetName() { return GetName(m_type); }

    virtual void Enter(Entity*);
    virtual void Leave(Entity*);
};