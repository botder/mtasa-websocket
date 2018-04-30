/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CLuaArguments.cpp
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "CLuaArguments.h"
#include "CLuaArgument.h"
#include "lua.hpp"

CLuaArguments::CLuaArguments(lua_State *L, int i)
{
    ReadStack(L, i);
}

void CLuaArguments::Push(CLuaArgumentPtr argument)
{
    m_Arguments.emplace_back(std::move(argument));
}

void CLuaArguments::ReadStack(lua_State *L, int i)
{
    const int top = lua_gettop(L);

    if (i < 0)
        i += top + 1;

    for (; i <= top; ++i)
    {
        CLuaArgumentPtr argument = GetArgument(L, i);

        if (argument)
            m_Arguments.emplace_back(std::move(argument));
    }
}

unsigned int CLuaArguments::PushStack(lua_State *L) const
{
    unsigned int numArguments = 0;

    for (const CLuaArgumentPtr &argument : m_Arguments)
        numArguments += argument->Push(L);

    return numArguments;
}

void CLuaArguments::Clear()
{
    std::vector<CLuaArgumentPtr>{}.swap(m_Arguments);
}

size_t CLuaArguments::Size() const
{
    return m_Arguments.size();
}

CLuaArgumentPtr CLuaArguments::Get(size_t index) const
{
    if (index >= m_Arguments.size())
        return nullptr;

    return m_Arguments[index];
}

CLuaArgumentPtr CLuaArguments::operator[](size_t index) const
{
    return Get(index);
}

CLuaArgumentPtr CLuaArguments::GetArgument(lua_State *L, int i)
{
    const int top = lua_gettop(L);

    if (i < 0)
        i += top + 1;

    CLuaArgumentPtr argument{};

    switch (lua_type(L, i))
    {
    case LUA_TNIL:
        argument = std::make_shared<CLuaNil>();
        break;
    case LUA_TBOOLEAN:
        argument = std::make_shared<CLuaBoolean>(lua_toboolean(L, i));
        break;
    case LUA_TNUMBER:
        argument = std::make_shared<CLuaNumber>(lua_tonumber(L, i));
        break;
    case LUA_TSTRING:
        argument = std::make_shared<CLuaString>(lua_tostring(L, i));
        break;
    case LUA_TTABLE:
        argument = std::make_shared<CLuaTable>(L, i);
        break;
    case LUA_TFUNCTION:
        argument = std::make_shared<CLuaFunction>(L, i);
        break;
    case LUA_TUSERDATA:
        argument = std::make_shared<CLuaUserData>(lua_touserdata(L, i));
        break;
    case LUA_TTHREAD:
        argument = std::make_shared<CLuaThread>(L, i);
        break;
    case LUA_TLIGHTUSERDATA:
    case LUA_TNONE:
    default:
        break;
    }

    return argument;
}
