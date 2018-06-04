/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        ResourceManager.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "ResourceManager.h"
#include "LuaWebsocket.h"

#include <algorithm>

ResourceManager::~ResourceManager()
{
	for (ResourceWebsocketsMap::reference pair : m_ResourceWebsockets)
	{
		for (LuaWebsocket* luaWebsocket : pair.second)
			luaWebsocket->OnResourceStop();
	}
}

void ResourceManager::AddWebsocket(const Resource& resource, LuaWebsocket& luaWebsocket)
{
	auto it = m_ResourceWebsockets.find(resource);

	if (it == m_ResourceWebsockets.end())
		return;

	it->second.emplace_back(&luaWebsocket);
}

void ResourceManager::RemoveWebsocket(const Resource& resource, const LuaWebsocket& luaWebsocket)
{
	auto it = m_ResourceWebsockets.find(resource);

	if (it == m_ResourceWebsockets.end())
		return;

	it->second.erase(std::remove(it->second.begin(), it->second.end(), &luaWebsocket));

	if (it->second.empty())
		m_ResourceWebsockets.erase(it);
}

void ResourceManager::OnResourceStop(const Resource& resource)
{
	auto it = m_ResourceWebsockets.find(resource);

	if (it == m_ResourceWebsockets.end())
		return;

	for (LuaWebsocket* luaWebsocket : it->second)
		luaWebsocket->OnResourceStop();

	m_ResourceWebsockets.erase(it);
}
