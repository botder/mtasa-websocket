/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Resource.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "stdafx.h"
#include "Resource.h"
#include "LuaWebsocket.h"

#include <sol.hpp>

Resource::Resource(lua_State* pLuaVM)
    : m_pLuaVM(pLuaVM)
{
}

void Resource::OnResourceStart()
{
    sol::state_view lua(m_pLuaVM);
    sol::table websocket = lua.create_named_table("ws");

    websocket.new_usertype<LuaWebsocket>("socket",
        sol::call_constructor, sol::constructors<LuaWebsocket(), LuaWebsocket(const std::string&)>(),
        "uri",                 sol::property(&LuaWebsocket::GetURI, &LuaWebsocket::SetURI),
        "open",                &LuaWebsocket::Open,
        "close",               &LuaWebsocket::Close,
        "send",                &LuaWebsocket::Send,
        "on",                  &LuaWebsocket::AddEventHandler,
        "state",               sol::readonly_property(&LuaWebsocket::GetState)
    );

    websocket.new_enum("FrameOpcode",
        "Continuation", LuaWebsocket::FrameOpcode::Continuation,
        "Text",         LuaWebsocket::FrameOpcode::Text,
        "Binary",       LuaWebsocket::FrameOpcode::Binary
    );

    websocket.new_enum("State",
        "Invalid",      LuaWebsocket::State::Invalid,
        "Connecting",   LuaWebsocket::State::Connecting,
        "Connected",    LuaWebsocket::State::Connected,
        "Closing",      LuaWebsocket::State::Closing,
        "Closed",       LuaWebsocket::State::Closed
    );
}

void Resource::OnResourceStop()
{
    sol::state_view lua(m_pLuaVM);
    lua["ws"] = sol::nil;
    m_pLuaVM = nullptr;
}
