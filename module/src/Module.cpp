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
#include "LuaWebsocket.h"
#include "WebsocketManager.h"
#include "ResourceManager.h"

#include <sol.hpp>
#include <cstring>
#include <cstdio>
#include <string>
#include <memory>
#include <thread>

static const std::string kModuleName    { "Websocket module" };
static const std::string kModuleAuthor  { "Octalype"         };
static const float       kModuleVersion { WSM_VERSION };

static const std::string USER_AGENT = WSM_USER_AGENT;
std::shared_ptr<WebsocketManager> g_WebsocketManager = nullptr;
std::unique_ptr<ResourceManager> g_ResourceManager = nullptr;

ILuaModuleManager10 *g_ModuleManager = nullptr;

MTAEXPORT bool InitModule(ILuaModuleManager10 *moduleManager, char *moduleName, char *moduleAuthor, float *moduleVersion)
{
    g_ModuleManager = moduleManager;

    std::strncpy(moduleName, kModuleName.c_str(), kMaxInfoLength);
    std::strncpy(moduleAuthor, kModuleAuthor.c_str(), kMaxInfoLength);
    *moduleVersion = kModuleVersion;

	try
	{
		g_WebsocketManager = std::make_shared<WebsocketManager>();
	}
	catch (std::exception &e)
	{
		moduleManager->ErrorPrintf("Failed to create an instance of WebsocketManager in InitModule: %s", e.what());
		return false;
	}

	try
	{
		g_ResourceManager = std::make_unique<ResourceManager>();
	}
	catch (std::exception &e)
	{
		moduleManager->ErrorPrintf("Failed to create an instance of ResourceManager in InitModule: %s", e.what());
		return false;
	}

	return ImportLua();
}

MTAEXPORT void RegisterFunctions(lua_State *luaState)
{
    if (!luaState)
        return;

	// Reset stack to avoid problems
    lua_settop(luaState, 0);

	// Create custom type
    sol::state_view lua{luaState};

	lua.new_usertype<LuaWebsocket>("websocket",
		sol::constructors<LuaWebsocket(sol::this_state), LuaWebsocket(sol::this_state, const std::string&)>(),
		"uri",           sol::property(&LuaWebsocket::GetURI, &LuaWebsocket::SetURI),
		"open",          &LuaWebsocket::Open,
		"close",         &LuaWebsocket::Close,
		"send",          &LuaWebsocket::Send,
		"onEvent",       &LuaWebsocket::AddEventHandler,
		"socketState",   sol::readonly_property(&LuaWebsocket::GetWebsocketState),
		"resourceState", sol::readonly_property(&LuaWebsocket::GetResourceState)
	);
}

MTAEXPORT bool DoPulse(void)
{
    return true;
}

MTAEXPORT void ResourceStopped(lua_State *luaState)
{
	g_ModuleManager->Printf("ResourceStopped\n");
	g_ResourceManager->OnResourceStop(luaState);
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

MTAEXPORT bool ShutdownModule(void)
{
	g_ModuleManager->Printf("ShutdownModule\n");
	g_ResourceManager.reset();
	g_WebsocketManager.reset();
	std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
}
