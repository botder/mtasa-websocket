/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        WebsocketManager.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include "ElementID.h"
#include "Websocket.h"

#include <vector>
#include <deque>

class WebsocketManager
{
public:
    WebsocketManager();

    ElementID  Create();
    Websocket* Get(ElementID id);
    bool       IsAlive(ElementID id);
    void       Destroy(ElementID id);

private:
    struct WebsocketData
    {
        std::uint8_t m_Generation;
        Websocket    m_Websocket;

        WebsocketData() : m_Generation(0) {}
    };

    std::deque<std::uint16_t>  m_FreeIndices;
    std::vector<WebsocketData> m_Data;
};
