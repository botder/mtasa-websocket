/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CConnectionManager.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include "CConnection.h"

#include <map>
#include <string>
#include <thread>
#include <memory>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

using CConnectionPtr = std::shared_ptr<CConnection>;
using CEndpoint = websocketpp::client<websocketpp::config::asio_tls_client>;
using SSLContextPtr = std::shared_ptr<websocketpp::lib::asio::ssl::context>;

class CConnectionManager
{
    using ConnectionMap = std::map<CEndpoint::connection_ptr, CConnection>;

public:
    // Tracking the state of the endpoint won't be neccessary when the
    // websocketpp library finally has a method to retrieve the internal state
    enum class ManagerState
    {
        UNINITIALIZED,
        READY,
        LISTENING,
    };

    CConnectionManager();
    ~CConnectionManager();
    ManagerState GetState() const { return m_State; }
    void CreateEndpoint(std::error_code& ec);
    void DestroyEndpoint(std::error_code& ec);
    CConnectionPtr CreateConnection(const std::string& uri, std::error_code& ec);
    void DestroyConnection(const CConnectionPtr& connection, std::error_code& ec);

private:
    void OpenHandler(websocketpp::connection_hdl hdl);
    void CloseHandler(websocketpp::connection_hdl hdl);
    void FailHandler(websocketpp::connection_hdl hdl);
    void HttpHandler(websocketpp::connection_hdl hdl);
    void MessageHandler(websocketpp::connection_hdl hdl, CEndpoint::message_ptr message);
    SSLContextPtr ContextHandler(websocketpp::connection_hdl hdl);

private:
    CEndpoint     m_Endpoint;
    ManagerState  m_State;
    std::thread   m_Thread;
    ConnectionMap m_Connections;
};
