#pragma once
#include "Entity/Entity.h"

class EntityPainter: public Entity
{
private:
    /* data */
public:
    static const TYPE type = Entity::painter;
    EntityPainter(/* args */);
    ~EntityPainter();
};

EntityPainter::EntityPainter(/* args */)
{
}

EntityPainter::~EntityPainter()
{
}
