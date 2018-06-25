/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        EndpointManager.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "stdafx.h"
#include "EndpointManager.h"
#include "Module.h"

using SSLContextPtr = std::shared_ptr<websocketpp::lib::asio::ssl::context>;
static const std::string USER_AGENT = WSM_USER_AGENT;

static SSLContextPtr ContextHandler(websocketpp::connection_hdl hdl)
{
    namespace asio = websocketpp::lib::asio;
    SSLContextPtr ctx = std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23_client);

    boost::system::error_code ec;

    ctx->set_options(asio::ssl::context::default_workarounds |
        asio::ssl::context::no_sslv2 |
        asio::ssl::context::no_sslv3 |
        asio::ssl::context::single_dh_use, ec);

    return ctx;
}

EndpointManager::EndpointManager()
{
    m_Endpoint.init_asio();
    m_Endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_Endpoint.clear_error_channels(websocketpp::log::elevel::all);
    m_Endpoint.set_tls_init_handler(&ContextHandler);
    m_Endpoint.set_user_agent(USER_AGENT);
}

EndpointManager::~EndpointManager()
{
    m_Endpoint.stop();
}

void EndpointManager::Run()
{
    m_Endpoint.run();
}
