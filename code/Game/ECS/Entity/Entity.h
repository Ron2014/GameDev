#pragma once

#include "Core/Object/ObjectManager.h"
#include "Component/Component.h"

using namespace Future;

class Entity
{
public:
    enum Type {
        player,
        monster,
        TypeCount,
    };
    
    static const std::string entity_names[Entity::TypeCount];

    static std::string GetName(Entity::Type type) {
        if (type < 0 || type >= Entity::TypeCount)
            return "";
        return entity_names[type];
    }

protected:
    ObjectManager<Component> m_mgrComponent;
    
    Entity::Type m_type;
    int m_ID;
    static int m_iNextValidID;
    
    Entity();

    void SetID(int id) { m_ID = id; }
    void SetType(Entity::Type type) { m_type = type; }

public:
    virtual ~Entity();

    int GetID() { return m_ID; }
    Entity::Type GetType() { return m_type; }
    std::string GetName() { return GetName(m_type); }

    virtual Component* GetComponent(Component::Type);
    virtual Component* AddComponent(Component::Type);
    virtual Component* RemoveComponent(Component::Type);
};