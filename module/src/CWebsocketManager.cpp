/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CWebsocketManager.cpp
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "CWebsocketManager.h"
#include <iostream>

namespace asio = websocketpp::lib::asio;
using ContextPtr = websocketpp::lib::shared_ptr<asio::ssl::context>;

CWebsocketManager::CWebsocketManager()
{
    m_Endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_Endpoint.clear_error_channels(websocketpp::log::elevel::all);

    m_Endpoint.set_open_handler([this](websocketpp::connection_hdl hdl) {
        this->OnOpen(hdl);
    });

    m_Endpoint.set_close_handler([this](websocketpp::connection_hdl hdl) {
        this->OnClose(hdl);
    });

    m_Endpoint.set_message_handler([this](websocketpp::connection_hdl hdl, Client::message_ptr& message) {
        this->OnMessage(hdl, message);
    });

    m_Endpoint.set_fail_handler([this](websocketpp::connection_hdl hdl) {
        this->OnError(hdl);
    });

    m_Endpoint.set_tls_init_handler([this](websocketpp::connection_hdl hdl) -> ContextPtr {
        ConnectionPtr con = m_Endpoint.get_con_from_hdl(hdl);

        if (!con)
        {
            std::cout << "CWebsocketManager::OnTLSInit failed to retrieve connection from HDL\n";
            return nullptr;
        }

        ContextPtr ctx = websocketpp::lib::make_shared<asio::ssl::context>(asio::ssl::context::sslv23_client);
        
        boost::system::error_code ec;
        ctx->set_options(asio::ssl::context::default_workarounds | 
                         asio::ssl::context::no_sslv2 |
                         asio::ssl::context::no_sslv3 |
                         asio::ssl::context::single_dh_use, ec);

        std::cout << "CWebsocketManager::OnTLSInit\n";

        return ctx;
    });

    m_Endpoint.init_asio();
    m_Endpoint.start_perpetual();

    m_Thread = websocketpp::lib::make_shared<Thread>(&Client::run, &m_Endpoint);
}

CWebsocketManager::~CWebsocketManager()
{
    m_Endpoint.stop_perpetual();

    for (const ConnectionPtr& con : m_Connections)
    {
        if (con->get_state() != websocketpp::session::state::open)
            continue;

        websocketpp::lib::error_code ec;
        con->close(websocketpp::close::status::going_away, "", ec);
    }

    m_Thread->join();
}

std::tuple<websocketpp::connection_hdl, websocketpp::lib::error_code> CWebsocketManager::GetConnection(const std::string& uri)
{
    websocketpp::lib::error_code ec;
    ConnectionPtr con = m_Endpoint.get_connection(uri, ec);

    if (ec)
        return std::make_tuple(websocketpp::connection_hdl{}, ec);

    m_Connections.emplace_back(con);

    return std::make_tuple(con->get_handle(), ec);
}

void CWebsocketManager::RemoveConnection(websocketpp::connection_hdl hdl)
{
    ConnectionPtr con = m_Endpoint.get_con_from_hdl(hdl);

    if (!con)
        return;

    m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), con));
}

bool CWebsocketManager::Connect(websocketpp::connection_hdl hdl)
{
    ConnectionPtr con = m_Endpoint.get_con_from_hdl(hdl);

    if (!con)
        return false;

    m_Endpoint.connect(con);
    return true;
}

bool CWebsocketManager::Close(websocketpp::connection_hdl hdl, const std::string& reason)
{
    ConnectionPtr con = m_Endpoint.get_con_from_hdl(hdl);

    if (!con || con->get_state() != websocketpp::session::state::open)
        return false;

    con->close(websocketpp::close::status::normal, reason);
    return true;
}

bool CWebsocketManager::Write(websocketpp::connection_hdl hdl, const std::string& payload)
{
    ConnectionPtr con = m_Endpoint.get_con_from_hdl(hdl);

    if (!con || con->get_state() != websocketpp::session::state::open)
        return false;

    return !con->send(payload, websocketpp::frame::opcode::TEXT);
}

void CWebsocketManager::OnOpen(websocketpp::connection_hdl hdl)
{
    ConnectionPtr con = m_Endpoint.get_con_from_hdl(hdl);

    if (!con)
    {
        std::cout << "CWebsocketManager::OnOpen failed to retrieve connection from HDL\n";
        return;
    }

    websocketpp::uri_ptr uri = con->get_uri();
    std::string host = uri->str();

    std::cout << "CWebsocketManager::OnOpen => " << con->get_uri()->get_host() << "\n";
}

void CWebsocketManager::OnClose(websocketpp::connection_hdl hdl)
{
    ConnectionPtr con = m_Endpoint.get_con_from_hdl(hdl);

    if (!con)
    {
        std::cout << "CWebsocketManager::OnClose failed to retrieve connection from HDL\n";
        return;
    }

    std::cout << "CWebsocketManager::OnClose\n";
}

void CWebsocketManager::OnMessage(websocketpp::connection_hdl hdl, Client::message_ptr& message)
{
    ConnectionPtr con = m_Endpoint.get_con_from_hdl(hdl);

    if (!con)
    {
        std::cout << "CWebsocketManager::OnMessage failed to retrieve connection from HDL\n";
        return;
    }

    const std::string& extension_data = message->get_extension_data();
    const std::string& header = message->get_header();
    websocketpp::frame::opcode::value opcode = message->get_opcode();
    const std::string& payload = message->get_payload();
    
    std::cout << "Opcode: " << opcode << "\n";
    std::cout << "Extension: " << extension_data << "\n";
    std::cout << "Header: " << header << "\n";
    std::cout << "Payload: " << payload << "\n";
}

void CWebsocketManager::OnError(websocketpp::connection_hdl hdl)
{
    ConnectionPtr con = m_Endpoint.get_con_from_hdl(hdl);

    if (!con)
    {
        std::cout << "CWebsocketManager::OnError failed to retrieve connection from HDL\n";
        return;
    }

    std::cout << "CWebsocketManager::OnError\n";
}
