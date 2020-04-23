#include "System.h"

const std::string System::system_names[System::TypeCount] = {
    "none",
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