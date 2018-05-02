/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CWebsocketManager.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#pragma warning(disable: 4267)

#include <vector>
#include <string>
#include <utility>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

class CWebsocketManager
{
    using Client        = websocketpp::client<websocketpp::config::asio_tls_client>;
    using ConnectionPtr = Client::connection_ptr;
    using Thread        = websocketpp::lib::thread;
    using ThreadPtr     = websocketpp::lib::shared_ptr<Thread>;

public:
    CWebsocketManager();
    ~CWebsocketManager();

    std::tuple<websocketpp::connection_hdl, websocketpp::lib::error_code> GetConnection(const std::string& uri);
    void RemoveConnection(websocketpp::connection_hdl hdl);
    bool Connect(websocketpp::connection_hdl hdl);
    bool Close(websocketpp::connection_hdl hdl, const std::string& reason);
    bool Write(websocketpp::connection_hdl hdl, const std::string& payload);

public:
    void OnOpen(websocketpp::connection_hdl hdl);
    void OnClose(websocketpp::connection_hdl hdl);
    void OnMessage(websocketpp::connection_hdl hdl, Client::message_ptr& message);
    void OnError(websocketpp::connection_hdl hdl);

private:
    Client                     m_Endpoint;
    ThreadPtr                  m_Thread;
    std::vector<ConnectionPtr> m_Connections;
};
