/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        LuaWebsocket.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include "ElementID.h"

#include <sol.hpp>

class LuaWebsocket
{
public:
    enum class FrameOpcode
    {
        Continuation = 0,
        Text         = 1,
        Binary       = 2,
    };

    enum class State
    {
        Invalid    = -1,
        Connecting = 0,
        Connected  = 1,
        Closing    = 2,
        Closed     = 3,
    };

    LuaWebsocket();
    LuaWebsocket(const std::string& uri);
    LuaWebsocket(const LuaWebsocket&) = default;
    LuaWebsocket& operator=(const LuaWebsocket&) = default;
    LuaWebsocket(LuaWebsocket&&) = default;
    LuaWebsocket& operator=(LuaWebsocket&&) = default;
    ~LuaWebsocket();

    void                       SetURI(const std::string& uri) const;
    sol::optional<std::string> GetURI() const;

    bool  Open();
    bool  Close(sol::optional<std::string> reason, sol::optional<int> code);
    bool  Send(const std::string& payload, sol::optional<FrameOpcode> opcode);
    bool  AddEventHandler(const char* eventName, sol::protected_function handler);
    State GetState() const;

private:
    void HandleInvokeResult(const char* eventName, sol::protected_function_result result);

    ElementID m_WebsocketID;

    sol::protected_function m_OpenHandler;
    sol::protected_function m_CloseHandler;
    sol::protected_function m_FailHandler;
    sol::protected_function m_MessageHandler;
    sol::protected_function m_ErrorHandler;
};
