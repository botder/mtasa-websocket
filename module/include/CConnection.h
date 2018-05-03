/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CConnection.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <string>
#include <websocketpp/common/connection_hdl.hpp>

class CConnection
{
    friend class CConnectionManager;

public:
    CConnection(websocketpp::connection_hdl&& hdl);
    websocketpp::connection_hdl GetHandle() const { return m_Handle; }

private:
    websocketpp::connection_hdl m_Handle;
};
