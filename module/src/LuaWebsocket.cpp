/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        LuaWebsocket.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "stdafx.h"
#include "LuaWebsocket.h"
#include "Module.h"
#include "WebsocketManager.h"

LuaWebsocket::LuaWebsocket()
    : m_WebsocketID(g_pWebsocketManager->Create())
{
    Websocket* pWebsocket = g_pWebsocketManager->Get(m_WebsocketID);

    if (pWebsocket)
    {
        pWebsocket->SetOpenHandler([this]() {
            if (m_OpenHandler)
                HandleInvokeResult("open", m_OpenHandler(this));
        });

        pWebsocket->SetCloseHandler([this](int code, const std::string& reason) {
            if (m_CloseHandler)
                HandleInvokeResult("close", m_CloseHandler(this, code, reason));
        });

        pWebsocket->SetFailHandler([this](int code, const std::string& reason) {
            if (m_FailHandler)
                HandleInvokeResult("fail", m_FailHandler(this, code, reason));
        });

        pWebsocket->SetMessageHandler([this](int opcode, const std::string& payload) {
            if (m_MessageHandler)
                HandleInvokeResult("message", m_MessageHandler(this, opcode, payload));
        });
    }
}

LuaWebsocket::LuaWebsocket(const std::string& uri)
    : LuaWebsocket()
{
    Websocket* pWebsocket = g_pWebsocketManager->Get(m_WebsocketID);

    if (pWebsocket)
        pWebsocket->SetURI(uri);
}

LuaWebsocket::~LuaWebsocket()
{
    Websocket* pWebsocket = g_pWebsocketManager->Get(m_WebsocketID);

    if (pWebsocket)
    {
        pWebsocket->SetOpenHandler(nullptr);
        pWebsocket->SetCloseHandler(nullptr);
        pWebsocket->SetFailHandler(nullptr);
        pWebsocket->SetMessageHandler(nullptr);
        pWebsocket->Close();
        g_pWebsocketManager->Destroy(m_WebsocketID);
        m_WebsocketID = ElementID::Invalid;
    }
}

void LuaWebsocket::SetURI(const std::string& uri) const
{
    Websocket* pWebsocket = g_pWebsocketManager->Get(m_WebsocketID);

    if (!pWebsocket)
        return;

    pWebsocket->SetURI(uri);
}

sol::optional<std::string> LuaWebsocket::GetURI() const
{
    Websocket* pWebsocket = g_pWebsocketManager->Get(m_WebsocketID);

    if (!pWebsocket)
        return sol::nullopt;

    return pWebsocket->GetURI();
}

bool LuaWebsocket::Open()
{
    Websocket* pWebsocket = g_pWebsocketManager->Get(m_WebsocketID);

    if (!pWebsocket)
        return false;

    return pWebsocket->Open();
}

bool LuaWebsocket::Close(sol::optional<std::string> reason, sol::optional<int> code)
{
    Websocket* pWebsocket = g_pWebsocketManager->Get(m_WebsocketID);

    if (!pWebsocket)
        return false;

    websocketpp::close::status::value closeStatus = websocketpp::close::status::going_away;

    if (code)
        closeStatus = static_cast<websocketpp::close::status::value>(code.value());

    if (reason)
        return pWebsocket->Close(reason.value(), closeStatus);
    else
        return pWebsocket->Close(closeStatus);
}

bool LuaWebsocket::Send(const std::string& payload, sol::optional<FrameOpcode> opcode)
{
    Websocket* pWebsocket = g_pWebsocketManager->Get(m_WebsocketID);

    if (!pWebsocket)
        return false;

    websocketpp::frame::opcode::value frameOpcode = websocketpp::frame::opcode::text;

    if (opcode)
        frameOpcode = static_cast<websocketpp::frame::opcode::value>(opcode.value());

    return pWebsocket->Send(payload, frameOpcode);
}

bool LuaWebsocket::AddEventHandler(const char* eventName, sol::protected_function handler)
{
    if (!std::strcmp(eventName, "open"))
        m_OpenHandler = handler;
    else if (!std::strcmp(eventName, "close"))
        m_CloseHandler = handler;
    else if (!std::strcmp(eventName, "fail"))
        m_FailHandler = handler;
    else if (!std::strcmp(eventName, "message"))
        m_MessageHandler = handler;
    else if (!std::strcmp(eventName, "error"))
        m_ErrorHandler = handler;
    else
        return false;

    return true;
}

LuaWebsocket::State LuaWebsocket::GetState() const
{
    Websocket* pWebsocket = g_pWebsocketManager->Get(m_WebsocketID);

    if (!pWebsocket)
       return State::Invalid;

    return static_cast<State>(pWebsocket->GetState());
}

void LuaWebsocket::HandleInvokeResult(const char* eventName, sol::protected_function_result result)
{
    if (m_ErrorHandler.valid() && !result.valid())
    {
        sol::error err = result;
        m_ErrorHandler(eventName, err.what());
    }
}
