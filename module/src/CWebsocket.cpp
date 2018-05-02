/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CWebsocket.cpp
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "CWebsocket.h"
#include "CWebsocketManager.h"
#include "Module.h"

CWebsocket::CWebsocket() = default;

CWebsocket::CWebsocket(const std::string& uri)
{
    Connect(uri);
}

CWebsocket::~CWebsocket()
{
    if (!m_HDL.expired())
        g_WebsocketManager->RemoveConnection(m_HDL);

    m_HDL.reset();
}

bool CWebsocket::Connect(const std::string& uri)
{
    if (!m_HDL.expired())
        Close();

    websocketpp::lib::error_code ec;
    std::tie(m_HDL, ec) = g_WebsocketManager->GetConnection(uri);

    if (m_HDL.expired())
        return false;

    return g_WebsocketManager->Connect(m_HDL);
}

bool CWebsocket::Write(const std::string& message)
{
    if (m_HDL.expired())
        return false;

    return g_WebsocketManager->Write(m_HDL, message);
}

bool CWebsocket::Close()
{
    if (m_HDL.expired())
        return false;

    bool success = g_WebsocketManager->Close(m_HDL, "");
    m_HDL.reset();
    return success;
}
