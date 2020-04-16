//------------------------------------------------------------------------------
//  Log.cc
//------------------------------------------------------------------------------
#include "StdFuture.h"
#include <cstdio>
#include "Core/Log/Log.h"
#include "Core/Assertion.h"
#include "Core/Log/Logger.h"
#include "Core/Log/StackTrace.h"

#if FUTURE_WINDOWS
#include <Windows.h>
#endif
#if FUTURE_ANDROID
#include <android/log.h>
#endif

#if FUTURE_HAS_THREADS
#include <mutex>
static std::mutex lockMutex;
#define SCOPED_LOCK std::lock_guard<std::mutex> lock(lockMutex)
#else
#define SCOPED_LOCK
#endif

#if FUTURE_WINDOWS
static const int LogBufSize = 4 * 1024;
#endif

namespace Future {

static Log::Level curLogLevel = Log::Level::Dbg;


std::vector<Ptr<Future::Logger>> Log::loggers;

//------------------------------------------------------------------------------
void
Log::AddLogger(const Ptr<Logger>& l) {
    if (l) {
        SCOPED_LOCK;
        loggers.emplace_back(l);
		l->StartLog();
    }
}

void
Log::RemoveLogger(const Ptr<Logger>& p) {
    if (p) {
        SCOPED_LOCK;
        for (auto iter = loggers.begin(); iter != loggers.end(); ++iter) {
            if (p == *iter) {
                loggers.erase(iter);
                break;
            }
        }
		p->EndLog();
    }
}
void Log::ClearLogger() {
	SCOPED_LOCK;
	for (auto iter = loggers.begin(); iter != loggers.end(); ++iter) {
		(*iter)->EndLog();
	}
	loggers.clear();
}
//------------------------------------------------------------------------------
size_t
Log::GetNumLoggers() {
    return loggers.size();
}

//------------------------------------------------------------------------------
Ptr<Logger>
Log::GetLogger(int index) {
    return loggers[index];
}

//------------------------------------------------------------------------------
void
Log::SetLogLevel(Level l) {
    curLogLevel = l;
}

//------------------------------------------------------------------------------
Log::Level
Log::GetLogLevel() {
    return curLogLevel;
}

//------------------------------------------------------------------------------
void
Log::Dbg(const char* msg, ...) {
    if (curLogLevel >= Level::Dbg) {
        va_list args;
        va_start(args, msg);
        Log::vprint(Level::Dbg, msg, args);
        va_end(args);
    }
}

//------------------------------------------------------------------------------
void
Log::VDbg(const char* msg, va_list args) {
    if (curLogLevel >= Level::Dbg) {
        Log::vprint(Level::Dbg, msg, args);
    }
}

//------------------------------------------------------------------------------
void
Log::Info(const char* msg, ...) {
    if (curLogLevel >= Level::Info) {
        va_list args;
        va_start(args, msg);
        Log::vprint(Level::Info, msg, args);
        va_end(args);
    }
}

//------------------------------------------------------------------------------
void
Log::VInfo(const char* msg, va_list args) {
    if (curLogLevel >= Level::Info) {
        Log::vprint(Level::Info, msg, args);
    }
}

//------------------------------------------------------------------------------
void
Log::Warn(const char* msg, ...) {
    if (curLogLevel >= Level::Warn) {
        va_list args;
        va_start(args, msg);
        Log::vprint(Level::Warn, msg, args);
        va_end(args);
    }
}

//------------------------------------------------------------------------------
void
Log::VWarn(const char* msg, va_list args) {
    if (curLogLevel >= Level::Warn) {
        Log::vprint(Level::Warn, msg, args);
    }
}

//------------------------------------------------------------------------------
void
Log::Error(const char* msg, ...) {
    if (curLogLevel >= Level::Error) {
        va_list args;
        va_start(args, msg);
        Log::vprint(Level::Error, msg, args);
        va_end(args);
    }
}

//------------------------------------------------------------------------------
void
Log::VError(const char* msg, va_list args) {
    if (curLogLevel >= Level::Error) {
        Log::vprint(Level::Error, msg, args);
    }
}

//------------------------------------------------------------------------------
void
Log::vprint(Level lvl, const char* msg, va_list args) {
    SCOPED_LOCK;
    if ( loggers.empty()) {
#if FUTURE_ANDROID
        android_LogPriority pri = ANDROID_LOG_DEFAULT;
        switch (lvl) {
        case Level::Error: pri = ANDROID_LOG_ERROR; break;
        case Level::Warn:  pri = ANDROID_LOG_WARN; break;
        case Level::Info:  pri = ANDROID_LOG_INFO; break;
        case Level::Dbg:   pri = ANDROID_LOG_DEBUG; break;
        default:           pri = ANDROID_LOG_DEFAULT; break;
        }
        __android_log_vprint(pri, "future", msg, args);
#else
#if FUTURE_WINDOWS
        va_list argsCopy;
        va_copy(argsCopy, args);
#endif

        // do the vprintf, this will destroy the original
        // va_list, so we made a copy before if necessary
        std::vprintf(msg, args);
        putchar('\n');

#if FUTURE_WINDOWS
        char buf[LogBufSize];
        auto size = std::vsnprintf(buf, sizeof(buf), msg, argsCopy);
#if FUTURE_WINDOWS
        if (size + 1 < LogBufSize) {
            buf[size] = '\n';
            buf[size + 1] = '\0';
        }
        else {
            buf[LogBufSize - 1] = 0;
        }
        
        OutputDebugStringA(buf);
#endif
#endif
#endif
    }
    else {
        for (auto l : loggers) {
            l->VPrint(lvl, msg, args);
        }
    }
}

void
Log::LogToLevel(Level l, const char* msg, ...) {
    if (curLogLevel >= l) {
        va_list args;
        va_start(args, msg);
        Log::vprint(l, msg, args);
        va_end(args);
    }
}

//------------------------------------------------------------------------------
void
Log::AssertMsg(const char* cond, const char* msg, const char* file, int line, const char* func) {
    SCOPED_LOCK;
    if ( loggers.empty()) {
        char callstack[4096];
        StackTrace::Dump(callstack, sizeof(callstack));
#if FUTURE_ANDROID
        __android_log_print(ANDROID_LOG_FATAL, "future", "\n\n*** FUTURE ASSERT: %s\n  msg: %s\n  file: %s\n  line: %d\n  func: %s\n  callstack:\n%s\n",
                            cond, msg ? msg : "none", file, line, func, callstack);
#else
        std::printf("\n\n*** FUTURE ASSERT: %s\n  msg=%s\n  file=%s\n  line=%d\n  func=%s\n  callstack:\n%s\n",
                    cond, msg ? msg : "none", file, line, func, callstack);
#if FUTURE_WINDOWS
        char buf[LogBufSize];
        _snprintf_s(buf, sizeof(buf), _TRUNCATE, "*** FUTURE ASSERT: %s\n  msg=%s\n  file=%s\n  line=%d\n  func=%s\n  callstack:\n%s\n",
                    cond, msg ? msg : "none", file, line, func, callstack);
        buf[LogBufSize - 1] = 0;
        OutputDebugStringA(buf);
#endif
#endif
    }
    else {
        for (const auto& l : loggers) {
            l->AssertMsg(cond, msg, file, line, func);
        }
    }
}

//------------------------------------------------------------------------------
const char*
Log::LevelToString(Level l) {
    switch (l) {
    case Level::Error: return "[E]";
    case Level::Warn: return "[W]";
    case Level::Info: return "[I]";
    case Level::Dbg: return "[D]";
    default: return "[?]";
    }
}

} // namespace Future
