#include "Entity.h"
#include "Global.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentHealthPoint.h"
#include "Component/ComponentMoving.h"
#include "Component/ComponentPathfinding.h"

#include <curses.h>

int Entity::m_iNextValidID = 0;

const std::string Entity::entity_names[Entity::TypeCount] = {
    "none",
    "player",
    "monster",
    "painter",
};

chtype entity_chtype[] = {
    0, 0, 0
};

Entity::Entity() {
    SetID(m_iNextValidID);
    m_iNextValidID++;
    if (m_iNextValidID < 0)
        m_iNextValidID = 0;
    
    gEntityMgr.AddMember(GetID(), this);
}

Entity::~Entity() {

}