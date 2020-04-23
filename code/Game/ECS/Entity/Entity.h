#pragma once

#include "Core/Log/Log.h"
#include "Core/Object/ObjectManager.h"
#include "Component/Component.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentMoving.h"
#include "Component/ComponentPathfinding.h"
#include "Component/ComponentHealthPoint.h"

using namespace Future;

class Entity
{
public:
    enum TYPE {
        none,
        player,
        monster,
        painter,
        TypeCount,
    };
    
    static const std::string entity_names[Entity::TypeCount];

    static std::string GetName(Entity::TYPE type) {
        if (type < 0 || type >= Entity::TypeCount)
            return "";
        return entity_names[type];
    }

protected:
    ObjectManager<Component::TYPE, Component> m_Components;
    
    int m_ID;
    static int m_iNextValidID;
    void SetID(int id) { m_ID = id; }
    
    Entity();

public:
    static const Entity::TYPE type = Entity::none;
    
    virtual ~Entity();

    int GetID() { return m_ID; }
    virtual Entity::TYPE GetType() { return type; }
    virtual std::string GetName() { return GetName(type); }

    template<class T>
    T* GetComponent() {
        Component::TYPE component_type = T::type;
        return (T*)m_Components.GetMember(component_type);
    }

    template<class T>
    T* AddComponent() {
        T* c = GetComponent<T>();
        if (c) return c;

        c = new T();
        if(c) {
            c->AddTo(this);
            m_Components.AddMember(T::type, (Component *)c);
        } else
            Log::Error("Component not exist!");
        return c;
    }

    template<class T>
    T* RemoveComponent() {
        T* c = GetComponent<T>();
        if (c) m_Components.RemoveMember(T::type);
        return c;
    }
};