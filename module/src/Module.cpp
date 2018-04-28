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
#include "CLuaModule.h"
#include "luaimports.h"
#include "ILuaModuleManager.h"
#include <cstring>
#include <cstdio>

static lua_CFunction        pAddEvent = nullptr;
static lua_CFunction        pTriggerEvent = nullptr;
static CLuaModule          *pWebsocketModule = nullptr;
static ILuaModuleManager10 *pModuleManager = nullptr;
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

    pWebsocketModule->Register(L);
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
