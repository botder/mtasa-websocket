/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CLuaArguments.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <vector>
#include <memory>
#include <tuple>

struct lua_State;
class CLuaArgument;
using CLuaArgumentPtr = std::shared_ptr<CLuaArgument>;

class CLuaArguments
{
public:
    CLuaArguments() = default;
    CLuaArguments(lua_State *L, int i = 1);
    void Push(CLuaArgumentPtr argument);
    void ReadStack(lua_State *L, int i = 1);
    unsigned int PushStack(lua_State *L) const;
    void Clear();
    size_t Size() const;
    CLuaArgumentPtr Get(size_t index) const;
    CLuaArgumentPtr operator[](size_t index) const;

public:
    static CLuaArgumentPtr GetArgument(lua_State *L, int i);

private:
    std::vector<CLuaArgumentPtr> m_Arguments;
};
