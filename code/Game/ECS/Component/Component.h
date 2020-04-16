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

    Component() {
        SetID(m_iNextValidID);
        m_iNextValidID++;
        if (m_iNextValidID < 0)
            m_iNextValidID = 0;
    }

    void SetID(int id) { m_ID = id; }
    void SetType(Component::Type type) { m_type = type; }

public:
    int GetID() { return m_ID; }
    Component::Type GetType() { return m_type; }
    std::string GetName() { return GetName(m_type); }

    virtual void AddTo(Entity*);
    virtual void RemoveFrom(Entity*);
};