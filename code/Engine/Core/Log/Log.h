#pragma once
#include <cstdarg>
#include <vector>
#include <stddef.h>

#include "Core/Config.h"

namespace Future {

class Logger;
template<class TYPE> class Ptr;

class Log {
public:
    /// log levels
    enum class Level {
        None,
        Error,
        Warn,
        Info,
        Dbg,

        NumLevels,
        InvalidLevel
    };

    /// add a logger object
    static void AddLogger(const Ptr<Logger>& p);
    /// remove a logger object
    static void RemoveLogger(const Ptr<Logger>& p);
	/// clear all logger
	static void ClearLogger();
    /// get number of loggers
    static size_t GetNumLoggers();
    /// get logger at index
    static Ptr<Logger> GetLogger(int index);

    /// set current log level
    static void SetLogLevel(Level l);
    /// get current log level
    static Level GetLogLevel();
    /// print a debug message
    static void Dbg(const char* msg, ...) __attribute__((format(printf, 1, 2)));
    /// print a debug message (with va_list)
    static void VDbg(const char* msg, va_list args);
    /// print an info message
    static void Info(const char* msg, ...) __attribute__((format(printf, 1, 2)));
    /// print an info message (with va_list)
    static void VInfo(const char* msg, va_list args);
    /// print a warning
    static void Warn(const char* msg, ...) __attribute__((format(printf, 1, 2)));
    /// print a warning (with va_list)
    static void VWarn(const char* msg, va_list args);
    /// print an error (use o_error() macro to also abort the program)
    static void Error(const char* msg, ...) __attribute__((format(printf, 1, 2)));
    /// print an error (with va_list)
    static void VError(const char* msg, va_list args);
    /// print message to level
    static void LogToLevel(Level l, const char* msg, ...) __attribute__((format(printf, 2, 3)));
    /// print an assert message
    static void AssertMsg(const char* cond, const char* msg, const char* file, int line, const char* func);
    /// level id to string
    static const char* LevelToString(Level l);

private:
    /// generic vprint-style method
    static void vprint(Level l, const char* msg, va_list args) __attribute__((format(printf, 2, 0)));

    static std::vector<Ptr<Logger>> loggers;
};

/// shortcut for Log::Dbg()
#define f_dbg(...) do { Future::Log::Dbg(__VA_ARGS__); } while(0)
/// shortcut for Log::Info()
#define f_info(...) do { Future::Log::Info(__VA_ARGS__); } while(0)
/// shortcut for Log::Warn()
#define f_warn(...) do { Future::Log::Warn(__VA_ARGS__); } while(0)
/// shortcut for Log::Error()
#define f_error(...) do { Future::Log::Error(__VA_ARGS__); FUTURE_TRAP(); } while(0)


} // namespace Future
