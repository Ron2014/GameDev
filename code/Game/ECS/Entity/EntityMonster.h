#pragma once
#include "Entity.h"

class EntityMonster: public Entity
{
private:
    /* data */
public:
    EntityMonster();
    ~EntityMonster();
};

EntityMonster::EntityMonster():Entity()
{
    SetType(Entity::monster);
}

EntityMonster::~EntityMonster()
{
}
