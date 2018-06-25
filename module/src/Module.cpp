/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Module.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "stdafx.h"
#include "Module.h"
#include "luaimports.h"
#include "ILuaModuleManager.h"
#include "EndpointManager.h"
#include "WebsocketManager.h"
#include "ResourceManager.h"

#include <memory>
#include <cstring>
#include <string>

static const std::string kModuleName    { "Websocket module" };
static const std::string kModuleAuthor  { "Octalype"         };
static const float       kModuleVersion { WSM_VERSION };

std::unique_ptr<EndpointManager>  g_pEndpointManager = nullptr;
std::unique_ptr<WebsocketManager> g_pWebsocketManager = nullptr;
std::unique_ptr<ResourceManager>  g_pResourceManager = nullptr;

ILuaModuleManager10 *g_pModuleManager = nullptr;

MTAEXPORT bool InitModule(ILuaModuleManager10 *moduleManager, char *moduleName, char *moduleAuthor, float *moduleVersion)
{
    g_pModuleManager = moduleManager;

    std::strncpy(moduleName, kModuleName.c_str(), kMaxInfoLength);
    std::strncpy(moduleAuthor, kModuleAuthor.c_str(), kMaxInfoLength);
    *moduleVersion = kModuleVersion;

    try
    {
        g_pEndpointManager = std::make_unique<EndpointManager>();
    }
    catch (std::exception& e)
    {
        moduleManager->ErrorPrintf("Failed to create an instance of EndpointManager in InitModule: %s", e.what());
        return false;
    }

    try
    {
        g_pWebsocketManager = std::make_unique<WebsocketManager>();
    }
    catch (std::exception& e)
    {
        moduleManager->ErrorPrintf("Failed to create an instance of WebsocketManager in InitModule: %s", e.what());
        return false;
    }

    try
    {
        g_pResourceManager = std::make_unique<ResourceManager>();
    }
    catch (std::exception& e)
    {
        moduleManager->ErrorPrintf("Failed to create an instance of ResourceManager in InitModule: %s", e.what());
        return false;
    }

    return ImportLua();
}

MTAEXPORT void RegisterFunctions(lua_State* pLuaVM)
{
    if (g_pResourceManager)
        g_pResourceManager->OnResourceStart(pLuaVM);
}

MTAEXPORT bool DoPulse(void)
{
    if (g_pEndpointManager)
        g_pEndpointManager->Run();

    return true;
}

MTAEXPORT void ResourceStopped(lua_State* pLuaVM)
{
    if (g_pResourceManager)
        g_pResourceManager->OnResourceStop(pLuaVM);
}

MTAEXPORT bool ShutdownModule(void)
{
    g_pResourceManager.reset();
    g_pWebsocketManager.reset();
    g_pEndpointManager.reset();
    return true;
}
