/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CLuaArgumentsBuilder.cpp
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "CLuaArgumentsBuilder.h"
#include "CLuaArgument.h"

void CLuaArgumentsBuilder::Clear()
{
    m_LuaArguments.Clear();
}

CLuaArguments& CLuaArgumentsBuilder::Get()
{
    return m_LuaArguments;
}

CLuaArgumentsBuilder& CLuaArgumentsBuilder::PushNil()
{
    m_LuaArguments.Push(std::make_shared<CLuaNil>());
    return *this;
}

CLuaArgumentsBuilder& CLuaArgumentsBuilder::PushBoolean(bool value)
{
    m_LuaArguments.Push(std::make_shared<CLuaBoolean>(value));
    return *this;
}

CLuaArgumentsBuilder& CLuaArgumentsBuilder::PushNumber(double value)
{
    m_LuaArguments.Push(std::make_shared<CLuaNumber>(value));
    return *this;
}

CLuaArgumentsBuilder& CLuaArgumentsBuilder::PushString(const char *value)
{
    m_LuaArguments.Push(std::make_shared<CLuaString>(value));
    return *this;
}

CLuaArgumentsBuilder& CLuaArgumentsBuilder::PushUserData(void *value)
{
    m_LuaArguments.Push(std::make_shared<CLuaUserData>(value));
    return *this;
}
