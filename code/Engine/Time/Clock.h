#pragma once
//------------------------------------------------------------------------------
/**
    @defgroup Time Time
    @brief time measurement and computations

    @class Future::Clock
    @ingroup Time
    @brief high-resolution time source
    
    The most important method of Clock is Now() which returns the 
    current point in time. The time values returned by Clock have
    no relation to the "wall-clock-time".
*/
#include <cstdint>

namespace Future {
    
class Clock {
public:
	static uint64_t GetRealTime();
};

} // namespace Future
