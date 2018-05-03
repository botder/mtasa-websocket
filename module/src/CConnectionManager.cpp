/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CConnectionManager.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "CConnectionManager.h"
#include "Module.h"

#include <algorithm>

static const std::string USER_AGENT = WSM_USER_AGENT;

CConnectionManager::CConnectionManager()
    : m_State(ManagerState::UNINITIALIZED)
{
}

CConnectionManager::~CConnectionManager()
{
    std::error_code ec;
    DestroyEndpoint(ec);
}

void CConnectionManager::CreateEndpoint(std::error_code& ec)
{
    ec.clear();

    if (m_State != ManagerState::UNINITIALIZED)
    {
        ec = websocketpp::error::make_error_code(websocketpp::error::endpoint_unavailable);
        return;
    }

    m_Endpoint.init_asio(ec);

    if (ec)
        return;

    m_Endpoint.clear_access_channels(websocketpp::log::alevel::none);
    m_Endpoint.clear_error_channels(websocketpp::log::elevel::none);

    using namespace std::placeholders;
    m_Endpoint.set_open_handler(std::bind(&CConnectionManager::OpenHandler, this, _1));
    m_Endpoint.set_close_handler(std::bind(&CConnectionManager::CloseHandler, this, _1));
    m_Endpoint.set_fail_handler(std::bind(&CConnectionManager::CloseHandler, this, _1));
    m_Endpoint.set_http_handler(std::bind(&CConnectionManager::HttpHandler, this, _1));
    m_Endpoint.set_message_handler(std::bind(&CConnectionManager::MessageHandler, this, _1, _2));
    m_Endpoint.set_tls_init_handler(std::bind(&CConnectionManager::ContextHandler, this, _1));
    m_Endpoint.set_user_agent(USER_AGENT);
    
    m_State = ManagerState::READY;
    m_Thread = std::thread{&CEndpoint::run, &m_Endpoint};
}

void CConnectionManager::DestroyEndpoint(std::error_code& ec)
{
    ec.clear();

    if (m_State == ManagerState::UNINITIALIZED)
    {
        ec = websocketpp::error::make_error_code(websocketpp::error::endpoint_unavailable);
        return;
    }

    if (m_State == ManagerState::LISTENING)
    {
        m_Endpoint.stop_perpetual();

        for (const ConnectionMap::const_reference connection : m_Connections)
        {
            // TODO
        }

        ConnectionMap{}.swap(m_Connections);
        m_Thread.join();
    }

    m_Endpoint.reset();
    m_State = ManagerState::UNINITIALIZED;
}

CConnectionPtr CConnectionManager::CreateConnection(const std::string& uri, std::error_code& ec)
{
    ec.clear();

    if (m_State == ManagerState::UNINITIALIZED)
    {
        ec = websocketpp::error::make_error_code(websocketpp::error::endpoint_unavailable);
        return nullptr;
    }

    CEndpoint::connection_ptr con = m_Endpoint.get_connection(uri, ec);

    if (ec)
        return nullptr;

    CConnectionPtr connection = std::make_shared<CConnection>(con->get_handle());
    m_Connections.emplace(std::make_pair(con, connection));

    return connection;
}

void CConnectionManager::DestroyConnection(const CConnectionPtr& connection, std::error_code& ec)
{
    ec.clear();

    if (m_State == ManagerState::UNINITIALIZED)
    {
        ec = websocketpp::error::make_error_code(websocketpp::error::endpoint_unavailable);
        return;
    }

    websocketpp::connection_hdl hdl = connection->GetHandle();
    CEndpoint::connection_ptr con = m_Endpoint.get_con_from_hdl(hdl, ec);

    if (ec)
        return;

    con->close(websocketpp::close::status::going_away, "", ec);
    m_Connections.erase(con);
}

void CConnectionManager::OpenHandler(websocketpp::connection_hdl hdl)
{
    // TODO
}

void CConnectionManager::CloseHandler(websocketpp::connection_hdl hdl)
{
    // TODO
}

void CConnectionManager::FailHandler(websocketpp::connection_hdl hdl)
{
    // TODO
}

void CConnectionManager::HttpHandler(websocketpp::connection_hdl hdl)
{
    // TODO
}

void CConnectionManager::MessageHandler(websocketpp::connection_hdl hdl, CEndpoint::message_ptr message)
{
    // TODO
}

SSLContextPtr CConnectionManager::ContextHandler(websocketpp::connection_hdl hdl)
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
