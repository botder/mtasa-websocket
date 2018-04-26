/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CSocketManager.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

class CSocket;

#include <vector>
#include <deque>
#include <assert.h>
#include "Module.h"

class CSocketManager
{
public:
    static void DoPulse();
    static void ResourceStopped( lua_State * luaVM );

    static void SocketAdd   (CSocket*& pSocket);
    static void SocketRemove(CSocket*& pSocket, bool bTriggerCloseEvent=true);
    static bool GetSocket   (CSocket*& pSocket, void* pUserdata);
    static bool GetSocketByLuaVM(CSocket*& pSocket, lua_State * luaVM);

    static void HandleStop  ();
};
