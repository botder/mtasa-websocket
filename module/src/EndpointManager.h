/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        EndpointManager.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

using ClientEndpoint = websocketpp::client<websocketpp::config::asio_tls_client>;

class EndpointManager
{
public:
    EndpointManager();
    EndpointManager(const EndpointManager&) = delete;
    EndpointManager& operator=(const EndpointManager&) = delete;
    EndpointManager(EndpointManager&& em) = default;
    EndpointManager& operator=(EndpointManager&& em) = default;
    ~EndpointManager();

    void            Run();
    ClientEndpoint& GetEndpoint() { return m_Endpoint; }

private:
    ClientEndpoint m_Endpoint;
};
