/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CConnection.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "CConnection.h"

CConnection::CConnection(websocketpp::connection_hdl&& hdl)
{
    m_Handle = std::forward<websocketpp::connection_hdl>(hdl);
}
