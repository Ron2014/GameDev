#include "System.h"

std::string system_names[System::TypeCount] = {
    "world_render",
    "motion_steer",
    "path_finding",
};

System::System() {

}

System::~System() {
    
}