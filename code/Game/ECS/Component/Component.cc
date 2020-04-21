#include "Component.h"
#include "Entity/Entity.h"

int Component::m_iNextValidID = 0;
std::string component_names[Component::TypeCount] = {
    "location",
    "health_point",
    "moving",
    "path_finding",
    "painter",
};

Component::Component() {
    SetID(m_iNextValidID);
    m_iNextValidID++;
    if (m_iNextValidID < 0)
        m_iNextValidID = 0;
}

void Component::AddTo(Entity *e) {

}

void Component::RemoveFrom(Entity *e) {

}