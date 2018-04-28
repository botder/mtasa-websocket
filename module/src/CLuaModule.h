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
#pragma once

#include <string>
#include <vector>

struct lua_State;
using lua_CFunction = int(*)(lua_State *L);

class CLuaModule
{
    struct Method
    {
        std::string name;
        lua_CFunction function;

        Method(const char *methodName, lua_CFunction function)
            : name(methodName)
            , function(function)
        {
        }
    };

public:
    CLuaModule(const char *moduleName);
    void AddMethod(const char *methodName, lua_CFunction function);
    void Register(lua_State *L);

private:
    std::string         m_ModuleName;
    std::vector<Method> m_Methods;
};
