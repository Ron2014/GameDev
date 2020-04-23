#pragma once
#include "Entity.h"

class EntityMonster: public Entity
{
private:
    /* data */
public:
    static const TYPE type = Entity::monster;
    EntityMonster();
    ~EntityMonster();
};

EntityMonster::EntityMonster():Entity()
{
}

EntityMonster::~EntityMonster()
{
}
