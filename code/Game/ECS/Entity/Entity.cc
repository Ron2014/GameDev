#include "Entity.h"
#include "Core/Log/Log.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentHealthPoint.h"
#include "Component/ComponentMoving.h"
#include "Component/ComponentPathfinding.h"

int Entity::m_iNextValidID = 0;

std::string entity_names[Entity::TypeCount] = {
    "player",
    "monster",
};

Entity::Entity() {
    SetID(m_iNextValidID);
    m_iNextValidID++;
    if (m_iNextValidID < 0)
        m_iNextValidID = 0;
}

Entity::~Entity() {

}

Component* Entity::GetComponent(Component::Type component_type) {
    return m_mgrComponent.GetMember((int)component_type);
}

Component* Entity::RemoveComponent(Component::Type component_type) {
    Component* c = GetComponent(component_type);
    m_mgrComponent.RemoveMember((int)component_type);
    return c;
}

Component* Entity::AddComponent(Component::Type component_type) {
    Component* c = GetComponent(component_type);
    if (c) return c;

    switch(component_type) {
        case Component::location:
            c = new ComponentLocation();
            break;
        case Component::health_point:
            c = new ComponentHealthPoint();
            break;
        case Component::path_finding:
            c = new ComponentPathfinding();
            break;
        case Component::moving:
            c = new ComponentMoving();
            break;
        default:
            break;
    }

    if(c) {
        c->AddTo(this);
        m_mgrComponent.AddMember(component_type, c);
    } else
        Log::Error("Component(%d) not exist!", component_type);
    return c;
}