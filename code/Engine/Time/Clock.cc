//------------------------------------------------------------------------------
//  Clock.cc
//------------------------------------------------------------------------------
#include "Clock.h"
#if FUTURE_WINDOWS
#include <Windows.h>
#else
#include <chrono>
#endif
#include "Core/Log/Log.h"

namespace Future {

#if defined(FUTURE_WINDOWS)
struct _perf {
    _perf() {
        ::QueryPerformanceFrequency(&this->freq);
        ::QueryPerformanceCounter(&this->start);
    };
    LARGE_INTEGER freq;
    LARGE_INTEGER start;
} perf;

uint64_t Clock::GetRealTime() {
    LARGE_INTEGER perfCount;
    QueryPerformanceCounter(&perfCount);
    long long t = ((perfCount.QuadPart - perf.start.QuadPart) * 1000) / perf.freq.QuadPart;
    return (uint64_t)t;
}

#elif defined(FUTURE_LINUX)
#include <stdio.h>
#include <time.h>
uint64_t Clock::GetRealTime() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ((uint64_t)ts.tv_sec * 1000 + ts.tv_nsec/(1000*1000));
}
#else
uint64_t Clock::GetRealTime() {
	timeval tv;
	gettimeofday(&tv, 0);
	return ((uint64_t)tv.tv_sec * 1000) + (tv.tv_usec / 1000);;
}
#endif

} // namespace Future
