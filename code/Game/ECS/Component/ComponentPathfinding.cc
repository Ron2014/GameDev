#include "ComponentPathfinding.h"
#include "System/SystemPathfinding.h"

ComponentPathfinding::ComponentPathfinding():head(nullptr),Component(){
    SetType(Component::path_finding);
}

ComponentPathfinding::~ComponentPathfinding() {
    PathNode *node;
    while(head) {
        node = head;
        head = head->next;
        free(node);            
    }
}

void ComponentPathfinding::SetDestination(const Vector3D& destination) {
    head = (PathNode *)malloc(sizeof(PathNode));
    head->point = destination;
    head->next = nullptr;
}

void ComponentPathfinding::AddTo(Entity *e) {
    SystemPathfinding::Instance()->RegisterEntity(e->GetID());
}

void ComponentPathfinding::RemoveFrom(Entity *e) {
    SystemPathfinding::Instance()->UnregisterEntity(e->GetID());
}