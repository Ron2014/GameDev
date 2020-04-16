#pragma once
#include "Entity.h"

class EntityPlayer: public Entity
{
private:
    /* data */
public:
    EntityPlayer(/* args */);
    ~EntityPlayer();
};

EntityPlayer::EntityPlayer(/* args */):Entity()
{
    SetType(Entity::player);
}

EntityPlayer::~EntityPlayer()
{
}
