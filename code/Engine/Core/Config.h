#pragma once

#if _DEBUG
#define FUTURE_DEBUG (1)
#endif

#ifndef __GNUC__
#define __attribute__(x)
#endif

// does the platform have std::thread and/or thread-local compiler support?
#if FUTURE_FORCE_NO_THREADS
#define FUTURE_HAS_THREADS (0)
#define FUTURE_COMPILER_HAS_THREADLOCAL (0)
#define FUTURE_THREADLOCAL_PTHREAD (0)
#elif FUTURE_EMSCRIPTEN
#define FUTURE_HAS_THREADS (0)
#define FUTURE_COMPILER_HAS_THREADLOCAL (0)
#define FUTURE_THREADLOCAL_PTHREAD (0)
#elif FUTURE_IOS
#define FUTURE_HAS_THREADS (1)
#define FUTURE_COMPILER_HAS_THREADLOCAL (0)
#define FUTURE_THREADLOCAL_PTHREAD (1)
#else
#define FUTURE_HAS_THREADS (1)
#define FUTURE_COMPILER_HAS_THREADLOCAL (1)
#define FUTURE_THREADLOCAL_PTHREAD (0)
#endif

/// silence unused variable warning
#define FUTURE_UNUSED __attribute__((unused))

// platform specific max-alignment
#if FUTURE_EMSCRIPTEN
#define FUTURE_MAX_PLATFORM_ALIGN (4)
#else
/// max memory alignment for this platform
#define FUTURE_MAX_PLATFORM_ALIGN (16)
#endif
