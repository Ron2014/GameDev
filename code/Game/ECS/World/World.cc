#include "World.h"

int World::m_iNextValidID = 0;
std::string world_names[World::TypeCount] = {
    "maintown",
    "battle",
};

World::World() {
    SetID(m_iNextValidID);
    m_iNextValidID++;
    if (m_iNextValidID < 0)
        m_iNextValidID = 0;
}

World::~World() {
    m_sEntityIDs.clear();
}

void World::Enter(Entity *e) {
    m_sEntityIDs.insert(e->GetID());
}

void World::Leave(Entity *e) {
    m_sEntityIDs.erase(e->GetID());
}
