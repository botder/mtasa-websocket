/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CLuaVM.cpp
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "CLuaVM.h"
#include "lua.hpp"

CLuaVM::CLuaVM(lua_State *L)
    : m_State(L)
{
}

CLuaFunction CLuaVM::GetFunction(const char *functionName)
{
    lua_pushvalue(m_State, LUA_GLOBALSINDEX);
    lua_pushstring(m_State, functionName);
    lua_gettable(m_State, -2);
    CLuaFunction function{m_State, -1};
    lua_pop(m_State, 2);
    return function;
}
