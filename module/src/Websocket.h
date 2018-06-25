/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Websocket.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include "EndpointManager.h"

#include <string>
#include <functional>

class Websocket
{
public:
    Websocket() = default;
    Websocket(const Websocket&) = delete;
    Websocket& operator=(const Websocket&) = delete;
    Websocket(Websocket&&) = default;
    Websocket& operator=(Websocket&&) = default;
    ~Websocket() = default;

    void               SetURI(const std::string& uri) { m_URI = uri; }
    const std::string& GetURI() const { return m_URI; }

    bool Open();
    bool Close();
    bool Close(const std::string& reason);
    bool Close(websocketpp::close::status::value code);
    bool Close(const std::string& reason, websocketpp::close::status::value code);
    bool Send(const std::string& payload, websocketpp::frame::opcode::value opcode);

    websocketpp::session::state::value GetState() const;

    using OpenHandler = std::function<void()>;
    void SetOpenHandler(OpenHandler handler) { m_OpenHandler = handler; }

    using CloseHandler = std::function<void(int, const std::string&)>;
    void SetCloseHandler(CloseHandler handler) { m_CloseHandler = handler; }

    using FailHandler = std::function<void(int, const std::string&)>;
    void SetFailHandler(FailHandler handler) { m_FailHandler = handler; }

    using MessageHandler = std::function<void(int, const std::string&)>;
    void SetMessageHandler(MessageHandler handler) { m_MessageHandler = handler; }

private:
    void ForceCloseConnection();

    using ConnectionPtr = ClientEndpoint::connection_ptr;

    ConnectionPtr           m_pConnection;
    std::string             m_URI;

    OpenHandler    m_OpenHandler;
    CloseHandler   m_CloseHandler;
    FailHandler    m_FailHandler;
    MessageHandler m_MessageHandler;
};
