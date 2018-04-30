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
#include "CLuaVM.h"
#include "CLuaModule.h"
#include "CLuaArgumentsBuilder.h"
#include "luaimports.h"
#include "ILuaModuleManager.h"
#include "lua.hpp"
#include <cstring>
#include <cstdio>
#include <string>

static CLuaModule           *pWebsocketModule = nullptr;
static ILuaModuleManager10  *pModuleManager = nullptr;
static const std::string    kModuleName    { "Websocket module" };
static const std::string    kModuleAuthor  { "Octalype"         };
static const float          kModuleVersion { 1.0f               };

static int Websocket_Open(lua_State *luaVM)
{
    lua_pushliteral(luaVM, "Websocket_Open");
    return 1;
}

MTAEXPORT bool InitModule(ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion)
{
    pModuleManager = pManager;

    std::strncpy(szModuleName, kModuleName.c_str(), kMaxInfoLength);
    std::strncpy(szAuthor, kModuleAuthor.c_str(), kMaxInfoLength);
    *fVersion = kModuleVersion;

    pWebsocketModule = new CLuaModule("websocket");
    pWebsocketModule->AddMethod("open", &Websocket_Open);

    return ImportLua();
}

MTAEXPORT void RegisterFunctions(lua_State *L)
{
    if (!pModuleManager || !pWebsocketModule || !L)
        return;

    lua_settop(L, 0);
    pWebsocketModule->Register(L);

    CLuaVM vm{L};
    CLuaArgumentsBuilder builder;
    builder.PushString("onWebsocketOpen");
    CLuaFunction outputServerLog = vm.GetFunction("outputServerLog");
    CLuaArguments results = outputServerLog(builder.Get());

    // auto [success] = results.Get<bool>();

    if (results.Size() == 1)
    {
        bool& success = reinterpret_cast<CLuaBoolean &>(*results[0]).GetValue();

        if (success)
            pModuleManager->Printf("outputServerLog() success!\n");
        else
            pModuleManager->Printf("outputServerLog() failed!\n");
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
    if (pWebsocketModule)
    {
        delete pWebsocketModule;
        pWebsocketModule = nullptr;
    }

    return true;
}
