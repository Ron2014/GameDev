#include "System.h"

std::string system_names[System::TypeCount] = {
    "world_render",
    "motion_steer",
    "path_finding",
    "moving",
    "painter",
};

System::System() {

}

System::~System() {
    
}