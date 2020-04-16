#pragma once

#include <set>
#include "Entity/Entity.h"
#include "Core/Singleton.h"
#include "Global.h"
#include "Core/Log/Log.h"

class System
{
public:
    enum Type {
        render,
        motion_steer,
        path_finding,
        TypeCount,
    };
    
    static const std::string system_names[System::TypeCount];

    static std::string GetName(System::Type type) {
        if (type < 0 || type >= System::TypeCount)
            return "";
        return system_names[type];
    }

protected:    
    System(){}

    System::Type m_type;
    void SetType(System::Type type) { m_type = type; }

public:
    virtual ~System();

    System::Type GetType() { return m_type; }
    std::string GetName() { return GetName(m_type); }

    virtual void Update()=0;
};

std::string system_names[System::TypeCount] = {
    "render",
    "motion_steer",
    "path_finding",
};