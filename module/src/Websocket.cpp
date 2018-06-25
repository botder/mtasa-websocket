/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Websocket.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "stdafx.h"
#include "Websocket.h"
#include "Module.h"

bool Websocket::Open()
{
    if (m_pConnection)
        ForceCloseConnection();

    ClientEndpoint& client = g_pEndpointManager->GetEndpoint();

    websocketpp::lib::error_code ec;
    m_pConnection = client.get_connection(m_URI, ec);

    if (ec)
        return false;

    m_pConnection->set_open_handler([this](websocketpp::connection_hdl) {
        if (m_OpenHandler)
            m_OpenHandler();
    });

    m_pConnection->set_close_handler([this](websocketpp::connection_hdl) {
        int code = m_pConnection->get_remote_close_code();
        const std::string reason = m_pConnection->get_remote_close_reason();

        m_pConnection.reset();

        if (m_CloseHandler)
            m_CloseHandler(code, reason);
    });

    m_pConnection->set_fail_handler([this](websocketpp::connection_hdl) {
        int code = m_pConnection->get_remote_close_code();
        const std::string reason = m_pConnection->get_remote_close_reason();

        m_pConnection.reset();

        if (m_FailHandler)
            m_FailHandler(code, reason);
    });

    m_pConnection->set_message_handler([this](websocketpp::connection_hdl, ClientEndpoint::message_ptr pMessage) {
        int opcode = pMessage->get_opcode();
        std::string payload;

        if (opcode == websocketpp::frame::opcode::text)
            payload = pMessage->get_payload();
        else
            payload = websocketpp::utility::to_hex(pMessage->get_payload());

        if (m_MessageHandler)
            m_MessageHandler(opcode, payload);
    });

    client.connect(m_pConnection);
    return true;
}

bool Websocket::Close()
{
    return Close("", websocketpp::close::status::going_away);
}

bool Websocket::Close(const std::string & reason)
{
    return Close(reason, websocketpp::close::status::going_away);
}

bool Websocket::Close(websocketpp::close::status::value code)
{
    return Close("", code);
}

bool Websocket::Close(const std::string& reason, websocketpp::close::status::value code)
{
    if (!m_pConnection)
        return false;

    const websocketpp::session::state::value sessionState = m_pConnection->get_state();

    if (sessionState == websocketpp::session::state::closing || sessionState == websocketpp::session::state::closed)
        return false;

    websocketpp::lib::error_code ec;
    m_pConnection->close(code, reason, ec);
    return true;
}

bool Websocket::Send(const std::string& payload, websocketpp::frame::opcode::value opcode)
{
    if (!m_pConnection)
        return false;

    if (m_pConnection->get_state() != websocketpp::session::state::open)
        return false;

    return !m_pConnection->send(payload, opcode);
}

websocketpp::session::state::value Websocket::GetState() const
{
    if (!m_pConnection)
        return websocketpp::session::state::closed;

    return m_pConnection->get_state();
}

void Websocket::ForceCloseConnection()
{
    if (!m_pConnection)
        return;

    m_pConnection->set_open_handler(nullptr);
    m_pConnection->set_close_handler(nullptr);
    m_pConnection->set_fail_handler(nullptr);
    m_pConnection->set_message_handler(nullptr);

    const websocketpp::session::state::value sessionState = m_pConnection->get_state();

    if (sessionState == websocketpp::session::state::open || sessionState == websocketpp::session::state::connecting)
    {
        websocketpp::lib::error_code ec;
        m_pConnection->close(websocketpp::close::status::abnormal_close, "", ec);
    }

    m_pConnection.reset();
}
