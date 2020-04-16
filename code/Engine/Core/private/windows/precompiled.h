#pragma once
//------------------------------------------------------------------------------
/**
    @file core/windows/precompiled.h
    
    Standard includes for Windows platform.
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <cstddef>

#include <windows.h>
#include <winbase.h>
#include <process.h>
#include <shlobj.h>
#include <tchar.h>
#include <strsafe.h>
#include <wininet.h>
#include <winsock2.h>
#include <rpc.h>
#include <dbghelp.h>
#include <intrin.h>