/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CWebsocket.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <string>
#include <websocketpp/common/connection_hdl.hpp>

class CWebsocket
{
public:
    CWebsocket();
    CWebsocket(const std::string& uri);
    ~CWebsocket();

public:
    bool Connect(const std::string& uri);
    bool Write(const std::string& message);
    bool Close();

private:
    websocketpp::connection_hdl m_HDL;
};
