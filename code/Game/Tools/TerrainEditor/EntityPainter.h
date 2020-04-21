#pragma once
#include "Entity/Entity.h"

class EntityPainter: public Entity
{
private:
    /* data */
public:
    EntityPainter(/* args */);
    ~EntityPainter();
};

EntityPainter::EntityPainter(/* args */)
{
    SetType(Entity::painter);
}

EntityPainter::~EntityPainter()
{
}
