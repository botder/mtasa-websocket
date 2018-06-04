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

#include <map>
#include <vector>
#include <memory>

class LuaWebsocket;

class ResourceManager
{
	using Resource = struct lua_State *;
	using ResourceWebsocketsMap = std::map<Resource, std::vector<LuaWebsocket *>>;

public:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	~ResourceManager();

public:
	void AddWebsocket(const Resource& resource, LuaWebsocket& luaWebsocket);
	void RemoveWebsocket(const Resource& resource, const LuaWebsocket& luaWebsocket);
	void OnResourceStop(const Resource& resource);

private:
	ResourceWebsocketsMap m_ResourceWebsockets;
};
