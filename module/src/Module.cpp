/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Module.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "Module.h"
#include "luaimports.h"
#include "ILuaModuleManager.h"
#include "CConnection.h"
#include "CConnectionManager.h"

#include <sol.hpp>
#include <cstring>
#include <cstdio>
#include <string>
#include <memory>

static const std::string kModuleName    { "Websocket module" };
static const std::string kModuleAuthor  { "Octalype"         };
static const float       kModuleVersion { WSM_VERSION };

static const std::string USER_AGENT = WSM_USER_AGENT;

ILuaModuleManager10 *g_ModuleManager = nullptr;
std::unique_ptr<CConnectionManager> g_ConnectionManager = nullptr;

MTAEXPORT bool InitModule(ILuaModuleManager10 *moduleManager, char *moduleName, char *moduleAuthor, float *moduleVersion)
{
    g_ModuleManager = moduleManager;

    std::strncpy(moduleName, kModuleName.c_str(), kMaxInfoLength);
    std::strncpy(moduleAuthor, kModuleAuthor.c_str(), kMaxInfoLength);
    *moduleVersion = kModuleVersion;

    try
    {
        g_ConnectionManager = std::make_unique<CConnectionManager>();
    }
    catch (std::exception &e)
    {
        moduleManager->ErrorPrintf("Failed to create an instance of CWebsocketManager in InitModule: %s", e.what());
        return false;
    }

    std::cout << "Using user-agent: " << USER_AGENT << '\n';

    return ImportLua();
}

MTAEXPORT void RegisterFunctions(lua_State *luaState)
{
    if (!luaState)
        return;

    lua_settop(luaState, 0);
    sol::state_view lua{luaState};

    /*lua.new_usertype<CWebsocket>("websocket",
        sol::constructors<CWebsocket(), CWebsocket(const std::string&)>(),
        "connect", &CWebsocket::Connect,
        "write", &CWebsocket::Write,
        "close", &CWebsocket::Close
    );*/

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
    g_ConnectionManager.reset();
    return true;
}
