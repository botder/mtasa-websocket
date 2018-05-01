/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CLuaVM.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include "CLuaArgument.h"

struct lua_State;

class CLuaVM
{
public:
    CLuaVM(lua_State *L);
    CLuaFunction GetFunction(const char *functionName);

private: 
    lua_State *m_State;
};