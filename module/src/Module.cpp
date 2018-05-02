/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Module.cpp
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "Module.h"
#include "luaimports.h"
#include "ILuaModuleManager.h"
#include "CWebsocket.h"
#include "CWebsocketManager.h"
#include <sol.hpp>
#include <functional>
#include <cstring>
#include <cstdio>
#include <string>

static const std::string kModuleName    { "Websocket module" };
static const std::string kModuleAuthor  { "Octalype"         };
static const float       kModuleVersion { 1.0f               };

ILuaModuleManager10 *g_ModuleManager = nullptr;
CWebsocketManager *g_WebsocketManager = nullptr;

MTAEXPORT bool InitModule(ILuaModuleManager10 *moduleManager, char *moduleName, char *moduleAuthor, float *moduleVersion)
{
    g_ModuleManager = moduleManager;

    std::strncpy(moduleName, kModuleName.c_str(), kMaxInfoLength);
    std::strncpy(moduleAuthor, kModuleAuthor.c_str(), kMaxInfoLength);
    *moduleVersion = kModuleVersion;

    try
    {
        g_WebsocketManager = new CWebsocketManager;
    }
    catch (std::exception &e)
    {
        moduleManager->ErrorPrintf("Failed to create an instance of CWebsocketManager in InitModule: %s", e.what());
        return false;
    }

    return ImportLua();
}

MTAEXPORT void RegisterFunctions(lua_State *luaState)
{
    if (!luaState)
        return;

    lua_settop(luaState, 0);
    sol::state_view lua{luaState};

    lua.new_usertype<CWebsocket>("websocket",
        sol::constructors<CWebsocket(), CWebsocket(const std::string&)>(),
        "connect", &CWebsocket::Connect,
        "write", &CWebsocket::Write,
        "close", &CWebsocket::Close
    );

    sol::function addEvent = lua["addEvent"];

    if (addEvent.get_type() == sol::type::function)
    {
        addEvent("websocket:onOpen");
        addEvent("websocket:onData");
        addEvent("websocket:onClose");
    }
}

MTAEXPORT bool DoPulse(void)
{
    return true;
}

MTAEXPORT void ResourceStopped(lua_State *luaVM)
{
}

MTAEXPORT bool ShutdownModule(void)
{
    return true;
}
