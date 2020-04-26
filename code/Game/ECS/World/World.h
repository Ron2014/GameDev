#pragma once
#include <set>

#include "Entity/Entity.h"
#include "Component/ComponentLocation.h"
#include "Terrain/TerrainConfig.h"

class World
{
public:
    enum TYPE {
        none,
        maintown,
        battle,       // copy scenes
        terrain_editor,
        TypeCount,
    };

    static const std::string world_names[World::TypeCount];

    static std::string GetName(World::TYPE type) {
        if (type < 0 || type >= World::TypeCount)
            return "";
        return world_names[type];
    }

protected:
    int m_ID;
    static int m_iNextValidID;

    // terrain
    std::string m_sTerrainName;

    // entitys
    std::set<int> m_sEntityIDs;

    World();

    void SetID(int id) { m_ID = id; }

public:
    static const World::TYPE type = World::none;
    virtual ~World();

    void LoadTerrain(const std::string &filename);
    TerrainConfig *GetTerrainConfig();
    TerrainConfig *LoadTempTerrain(const std::string &filename = TerrainConfig::DEFAULT_NAME);

    std::string &GetTerrainFilename() { return m_sTerrainName; }
    std::set<int>& GetEntityIDs() { return m_sEntityIDs; }
    int GetID() { return m_ID; }

    virtual World::TYPE GetType() { return type; }
    virtual std::string GetName() { return GetName(type); }

    virtual void Enter(Entity*);
    virtual void Leave(Entity*);
};