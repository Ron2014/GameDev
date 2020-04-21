#pragma once
#include <string>

class Entity;

struct Component
{
public:
    enum Type {
        location,
        health_point,
        moving,
        path_finding,
        painter,
        TypeCount,
    };
    
    static const std::string component_names[Component::TypeCount];

    static std::string GetName(Component::Type type) {
        if (type < 0 || type >= Component::TypeCount)
            return "";
        return component_names[type];
    }

protected:
    Component::Type m_type;
    int m_ID;
    static int m_iNextValidID;

    Component();
    
    void SetID(int id) { m_ID = id; }
    void SetType(Component::Type type) { m_type = type; }

public:
    int GetID() { return m_ID; }
    Component::Type GetType() { return m_type; }
    std::string GetName() { return GetName(m_type); }

    virtual void AddTo(Entity*);
    virtual void RemoveFrom(Entity*);
};