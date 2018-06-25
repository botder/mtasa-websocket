/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        ResourceManager.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "stdafx.h"
#include "ResourceManager.h"
#include "Resource.h"

#include "ILuaModuleManager.h"
#include "Module.h"

ResourceManager::~ResourceManager()
{
    g_pModuleManager->Printf("ResourceManager::~ResourceManager()\n");

    if (!m_Resources.empty())
    {
        bool wtf = true;
    }
}

 void ResourceManager::OnResourceStart(lua_State* pLuaVM)
{
    ResourcePtrMap::iterator it = m_Resources.find(pLuaVM);

    if (it != m_Resources.end())
        return;

    ResourcePtr pResource;

    try
    {
        pResource = std::make_shared<Resource>(pLuaVM);
    }
    catch (std::bad_alloc&)
    {
        return;
    }

    pResource->OnResourceStart();
    m_Resources.emplace(ResourcePtrMap::value_type(pLuaVM, std::move(pResource)));
}

void ResourceManager::OnResourceStop(lua_State* pLuaVM)
{
    ResourcePtrMap::iterator it = m_Resources.find(pLuaVM);

    if (it == m_Resources.end())
        return;

    it->second->OnResourceStop();
    m_Resources.erase(it);
}
