#pragma once

#include "Core/Log/Log.h"
#include <stdlib.h> // for abort()

#define FUTURE_TRAP() {int*x = nullptr; *x=1;}

#if !(__GNUC__ || __GNUC__)
// on Visual Studio, replace __PRETTY_FUNCTION__ with __FUNCSIG__
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

// assert macros
#if (__clang_analyzer__ || FUTURE_USE_STDASSERT)
#include <assert.h>
#define f_assert(cond) assert(cond)
#define f_assert2(cond, msg) assert(cond)
#define f_assert_dbg(cond) assert(cond)
#define f_assert2_dbg(cond, msg) assert(cond)
#define f_assert_range(val, max) assert((val >= 0) && (val < max))
#define f_assert_range_dbg(val, max) assert((val >= 0) && (val < max))
#elif !FUTURE_DEBUG && FUTURE_NO_ASSERT
#define f_assert(cond) ((void)0)
#define f_assert2(cond, msg) ((void)0)
#define f_assert_range(val, max) ((void)0)
#define f_assert_dbg(cond) ((void)0)
#define f_assert2_dbg(cond, msg) ((void)0)
#define f_assert_range_dbg(val, max) ((void)0)
#else
/// standard assert macro
#define f_assert(cond) do { if(!(cond)) { Future::Log::AssertMsg(#cond,nullptr,__FILE__,__LINE__,__PRETTY_FUNCTION__); FUTURE_TRAP(); } } while(0)
/// assert macro which is only active in debug mode
#if FUTURE_DEBUG
#define f_assert_dbg(cond) do { if(!(cond)) { Future::Log::AssertMsg(#cond,nullptr,__FILE__,__LINE__,__PRETTY_FUNCTION__); FUTURE_TRAP(); } } while(0)
#else
#define f_assert_dbg(cond) ((void)0)
#endif
/// assert macro with programmer's message
#define f_assert2(cond, msg) do { if(!(cond)) { Future::Log::AssertMsg(#cond,msg,__FILE__,__LINE__,__PRETTY_FUNCTION__); FUTURE_TRAP(); } } while(0)
#if FUTURE_DEBUG
#define f_assert2_dbg(cond, msg) do { if(!(cond)) { Future::Log::AssertMsg(#cond,msg,__FILE__,__LINE__,__PRETTY_FUNCTION__); FUTURE_TRAP(); } } while(0)
#else
#define f_assert2_dbg(cond, msg) ((void)0)
#endif
/// perform a range check (x >= 0) && (x < max)
#define f_assert_range(val, max) f_assert((val >= 0) && (val < max))
#define f_assert_range_dbg(val, max) f_assert_dbg((val >= 0) && (val < max))
#endif

