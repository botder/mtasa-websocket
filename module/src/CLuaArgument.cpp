/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CLuaArgument.cpp
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "CLuaArgument.h"
#include "lua.hpp"

unsigned int CLuaArgument::Push(lua_State *)
{
    return 0;
}

int CLuaArgument::GetType()
{
    return LUA_TNONE;
}

bool CLuaArgument::HasValue()
{
    return GetType() != LUA_TNONE;
}

unsigned int CLuaNil::Push(lua_State *L)
{
    lua_pushnil(L);
    return 1;
}

int CLuaNil::GetType()
{
    return LUA_TNIL;
}

unsigned int CLuaNumber::Push(lua_State *L)
{
    lua_pushnumber(L, static_cast<lua_Number>(m_Value));
    return 1;
}

int CLuaNumber::GetType()
{
    return LUA_TNUMBER;
}

unsigned int CLuaBoolean::Push(lua_State *L)
{
    lua_pushboolean(L, m_Value);
    return 1;
}

int CLuaBoolean::GetType()
{
    return LUA_TBOOLEAN;
}

unsigned int CLuaString::Push(lua_State *L)
{
    lua_pushlstring(L, m_Value.c_str(), m_Value.size());
    return 1;
}

int CLuaString::GetType()
{
    return LUA_TSTRING;
}

unsigned int CLuaUserData::Push(lua_State *L)
{
    lua_pushlightuserdata(L, m_Value);
    return 1;
}

int CLuaUserData::GetType()
{
    return LUA_TLIGHTUSERDATA;
}

CLuaReference::CLuaReference()
    : CLuaType(LUA_NOREF)
    , m_Type(LUA_TNONE)
    , m_State(nullptr)
{
}

CLuaReference::CLuaReference(lua_State *L, int i)
    : CLuaReference()
{
    m_State = L;
    const int top = lua_gettop(L);

    if (i < 0)
        i += top + 1;

    lua_pushvalue(L, i);
    m_Type = lua_type(L, -1);
    m_Value = luaL_ref(L, LUA_REGISTRYINDEX);
}

CLuaReference::~CLuaReference()
{
    if (m_State && m_Type != LUA_TNONE)
        luaL_unref(m_State, LUA_REGISTRYINDEX, m_Value);
}

CLuaReference::CLuaReference(CLuaReference &&other)
    : CLuaReference()
{
    *this = std::move(other);
}

CLuaReference &CLuaReference::operator=(CLuaReference &&other)
{
    m_Value = other.m_Value;
    m_Type = other.m_Type;
    m_State = other.m_State;
    other.m_Value = LUA_NOREF;
    other.m_Type = LUA_TNONE;
    other.m_State = nullptr;
    return *this;
}

int CLuaReference::GetType()
{
    return LUA_TNONE;
}

int CLuaReference::GetReferenceType()
{
    return m_Type;
}

unsigned int CLuaTable::Push(lua_State *L)
{
    if (L != m_State || m_Type != LUA_TTABLE)
    {
        // Copy from origin Lua state
        lua_pushnil(L);
        return 1;
    }

    lua_rawgeti(L, LUA_REGISTRYINDEX, m_Value);
    return 1;
}

int CLuaTable::GetType()
{
    return LUA_TTABLE;
}

unsigned int CLuaFunction::Push(lua_State *L)
{
    if (m_Type != LUA_TFUNCTION)
    {
        lua_pushnil(L);
        return 1;
    }

    lua_rawgeti(m_State, LUA_REGISTRYINDEX, m_Value);

    if (lua_iscfunction(m_State, -1))
    {
        const lua_CFunction function = lua_tocfunction(m_State, -1);

        if (function)
            lua_pushcfunction(L, function);
        else
            lua_pushnil(L);

        lua_pop(m_State, 1);
        return 1;
    }
    else if (lua_isfunction(m_State, -1))
    {
        return 1;
    }

    if (L != m_State)
    {
        // You can't copy a Lua function from one state to another
        lua_pushnil(L);
        return 1;
    }

    return 1;
}

int CLuaFunction::GetType()
{
    return LUA_TFUNCTION;
}

CLuaArguments CLuaFunction::Call(const CLuaArguments &arguments)
{
    return Call(m_State, arguments);
}

CLuaArguments CLuaFunction::Call(lua_State *L, const CLuaArguments &arguments)
{
    const int top = lua_gettop(L);
    
    if (Push(L) != 1)
    {
        lua_settop(L, top);
        return CLuaArguments{};
    }

    if (!lua_isfunction(L, -1))
    {
        lua_settop(L, top);
        return CLuaArguments{};
    }

    const unsigned int nargs = arguments.PushStack(L);
    const int errorCode = lua_pcall(L, nargs, LUA_MULTRET, NULL);

    if (errorCode != 0)
    {
        lua_settop(L, top);
        return CLuaArguments{};
    }

    CLuaArguments results{L, top + 1};
    lua_settop(L, top);

    return results;
}

CLuaArguments CLuaFunction::operator()(const CLuaArguments &arguments)
{
    return Call(m_State, arguments);
}

CLuaArguments CLuaFunction::operator()(lua_State *L, const CLuaArguments &arguments)
{
    return Call(L, arguments);
}

unsigned int CLuaThread::Push(lua_State *L)
{
    if (L != m_State || m_Type != LUA_TTHREAD)
    {
        // Copy from origin Lua state
        lua_pushnil(L);
        return 1;
    }

    lua_rawgeti(L, LUA_REGISTRYINDEX, m_Value);
    return 1;
}

int CLuaThread::GetType()
{
    return LUA_TTHREAD;
}
