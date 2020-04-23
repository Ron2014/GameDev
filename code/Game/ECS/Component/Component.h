#pragma once
#include <string>

class Entity;

struct Component
{
public:
    enum TYPE {
        none,
        location,
        health_point,
        moving,
        path_finding,
        painter,
        TypeCount,
    };
    
    static const std::string component_names[Component::TypeCount];

    static std::string GetName(Component::TYPE type) {
        if (type < 0 || type >= Component::TypeCount)
            return "";
        return component_names[type];
    }

protected:
    int m_ID;
    static int m_iNextValidID;
    void SetID(int id) { m_ID = id; }

    Component();

public:
    static const TYPE type = Component::none;
    int GetID() { return m_ID; }
    
    virtual Component::TYPE GetType() { return type; }
    virtual std::string GetName() { return GetName(type); }

    virtual void AddTo(Entity*);
    virtual void RemoveFrom(Entity*);
};