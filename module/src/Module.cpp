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

#pragma warning(disable: 4267)

#include "Module.h"
#include "luaimports.h"
#include "ILuaModuleManager.h"
#include <sol/sol.hpp>
#include <cstring>
#include <cstdio>
#include <string>

static const std::string kModuleName    { "Websocket module" };
static const std::string kModuleAuthor  { "Octalype"         };
static const float       kModuleVersion { 1.0f               };

static std::string Websocket_Open()
{
    return std::string{"Websocket_Open"};
}

MTAEXPORT bool InitModule(ILuaModuleManager10 *, char *moduleName, char *moduleAuthor, float *moduleVersion)
{
    std::strncpy(moduleName, kModuleName.c_str(), kMaxInfoLength);
    std::strncpy(moduleAuthor, kModuleAuthor.c_str(), kMaxInfoLength);
    *moduleVersion = kModuleVersion;

    return ImportLua();
}

MTAEXPORT void RegisterFunctions(lua_State *luaState)
{
    if (!luaState)
        return;

    lua_settop(luaState, 0);
    sol::state_view lua{luaState};
    sol::table websocket = lua.create_named_table("websocket");
    websocket.set_function("open", &Websocket_Open);
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
