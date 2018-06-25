/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        WebsocketManager.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "stdafx.h"
#include "WebsocketManager.h"

#include <limits>
#include <cassert>

static constexpr int MaxWebsockets = std::numeric_limits<std::uint16_t>::max();
static constexpr int IndexRecyclingThreshold = 1024;

WebsocketManager::WebsocketManager()
{
    m_Data.reserve(MaxWebsockets);
}

ElementID WebsocketManager::Create()
{
    std::size_t index;

    if (m_FreeIndices.size() > IndexRecyclingThreshold
        || (m_Data.size() == m_Data.capacity() && !m_FreeIndices.empty()))
    {
        index = m_FreeIndices.front();
        m_FreeIndices.pop_front();
    }
    else
    {
        if (m_Data.size() == m_Data.capacity())
            return ElementID::Invalid;

        m_Data.emplace_back();
        index = m_Data.size() - 1;
    }

    return ElementID(index, m_Data[index].m_Generation);
}

Websocket* WebsocketManager::Get(ElementID id)
{
    if (!IsAlive(id))
        return nullptr;

    return &m_Data[id.GetIndex()].m_Websocket;
}

bool WebsocketManager::IsAlive(ElementID id)
{
    if (id == ElementID::Invalid)
        return false;

    const std::uint16_t index = id.GetIndex();

    if (index >= m_Data.size())
        return false;

    return m_Data[index].m_Generation == id.GetGeneration();
}

void WebsocketManager::Destroy(ElementID id)
{
    assert(IsAlive(id));
    m_Data[id.GetIndex()].m_Generation += 1;
    m_FreeIndices.emplace_back(id.GetIndex());
}
