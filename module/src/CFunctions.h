/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CFunctions.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

class CFunctions;

#pragma once

#include <stdio.h>

#include "Module.h"
#include "ILuaModuleManager.h"
extern ILuaModuleManager10 *pModuleManager;

class CFunctions
{
public:
    static int sockOpen  (lua_State* luaVM);
    static int sockWrite (lua_State* luaVM);
    static int sockClose (lua_State* luaVM);

    static void AddEvent (lua_State* luaVM, const string& strEventName);
};
