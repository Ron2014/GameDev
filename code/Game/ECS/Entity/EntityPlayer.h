#pragma once
#include "Entity.h"

class EntityPlayer: public Entity
{
private:
    /* data */
public:
    static const TYPE type = Entity::player;
    EntityPlayer(/* args */);
    ~EntityPlayer();
};

EntityPlayer::EntityPlayer(/* args */):Entity()
{
}

EntityPlayer::~EntityPlayer()
{
}
