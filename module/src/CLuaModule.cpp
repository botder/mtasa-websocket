/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CLuaModule.cpp
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "CLuaModule.h"
#include "lua.hpp"

CLuaModule::CLuaModule(const char *moduleName)
    : m_ModuleName(moduleName)
{
}

void CLuaModule::AddMethod(const char *methodName, lua_CFunction function)
{
    m_Methods.emplace_back(methodName, function);
}

void CLuaModule::Register(lua_State *L)
{
    lua_pushvalue(L, LUA_GLOBALSINDEX); // _G
    lua_newtable(L); // table, _G

    for (Method &method : m_Methods)
    {
        lua_pushlstring(L, method.name.c_str(), method.name.size()); // string, table, _G
        lua_pushcfunction(L, method.function); // function, string, table, _G
        lua_settable(L, -3); // table, _G
    }

    lua_pushlstring(L, m_ModuleName.c_str(), m_ModuleName.size()); // string, table, _G
    lua_insert(L, -2); // table, string, _G
    lua_settable(L, -3); // _G

    lua_pop(L, 1);
}
