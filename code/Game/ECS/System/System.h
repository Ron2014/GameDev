#pragma once

#include <set>
#include "Entity/Entity.h"
#include "Core/Singleton.h"
#include "Global.h"
#include "Core/Log/Log.h"

class System
{
public:
    enum TYPE {
        none,
        world_render,
        motion_steer,
        path_finding,
        moving,
        painter,
        TypeCount,
    };
    
    static const std::string system_names[System::TypeCount];

    static std::string GetName(System::TYPE type) {
        if (type < 0 || type >= System::TypeCount)
            return "";
        return system_names[type];
    }

protected:    
    System();

public:
    static const System::TYPE type = System::none;
    virtual ~System();

    virtual System::TYPE GetType() { return type; }
    virtual std::string GetName() { return GetName(type); }

    virtual void Update()=0;
};