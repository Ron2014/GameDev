#include "World.h"

int World::m_iNextValidID = 0;
std::string world_names[World::TypeCount] = {
    "maintown",
    "battle",
};

void World::Enter(Entity *e) {
    m_sEntityIDs.insert(e->GetID());
}

void World::Leave(Entity *e) {
    m_sEntityIDs.erase(e->GetID());
}
