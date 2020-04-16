#include "Component.h"
#include "Entity/Entity.h"

int Component::m_iNextValidID = 0;
std::string component_names[Component::TypeCount] = {
    "location",
    "health_point",
    "moving",
    "path_finding",
};

void AddTo(Entity *e) {

}

void RemoveFrom(Entity *e) {

}