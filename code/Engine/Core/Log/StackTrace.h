#pragma once
//------------------------------------------------------------------------------
/**
@class Future::StackTrace
@brief low-level helper functions to dump a stack trace
*/

namespace Future {

class StackTrace {
public:
    /// write stack trace into buf as human-readable string 
    static void Dump(char* buf, int bufSize);
};

} // namespace Future
