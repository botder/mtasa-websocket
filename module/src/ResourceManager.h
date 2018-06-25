/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        ResourceManager.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include "Resource.h"

#include <map>
#include <memory>

struct lua_State;

class ResourceManager
{
public:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = default;
    ResourceManager& operator=(ResourceManager&&) = default;
    ~ResourceManager();

    void OnResourceStart(lua_State* pLuaVM);
    void OnResourceStop(lua_State* pLuaVM);

private:
    using ResourcePtr = std::shared_ptr<Resource>;
    using ResourcePtrMap = std::map<lua_State *, ResourcePtr>;

    ResourcePtrMap m_Resources;
};
