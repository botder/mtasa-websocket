/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Module.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <memory>
#include <boost/version.hpp>

#ifdef WIN32
#	define MTAEXPORT extern "C" __declspec(dllexport)
#else
#	define MTAEXPORT extern "C"
#endif

#define WSM_VERSION_MAJOR 1
#define WSM_VERSION_MINOR 0
#define WSM_VERSION_PATCH 0

#define WSM_QUOTE2(c) #c
#define WSM_QUOTE(x) WSM_QUOTE2(x)
#define WSM_VALUE(x) x

#define WSM_VERSION      WSM_VALUE(WSM_VERSION_MAJOR).WSM_VALUE(WSM_VERSION_MINOR)WSM_VALUE(WSM_VERSION_PATCH)
#define WSM_VERSION_TEXT WSM_QUOTE(WSM_VERSION_MAJOR) "." WSM_QUOTE(WSM_VERSION_MINOR) "." WSM_QUOTE(WSM_VERSION_PATCH)
#define WSM_USER_AGENT   "MTA:SA Server - Websocket (" WSM_VERSION_TEXT ") " BOOST_PLATFORM " (" BOOST_COMPILER ", Boost/" WSM_QUOTE(BOOST_VERSION) ")"

extern class ILuaModuleManager10 *g_ModuleManager;
extern std::unique_ptr<class CConnectionManager> g_ConnectionManager;
