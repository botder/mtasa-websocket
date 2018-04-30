/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CLuaArgumentsBuilder.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include "CLuaArguments.h"

class CLuaArgumentsBuilder
{
public:
    void Clear();
    CLuaArguments& Get();
    CLuaArgumentsBuilder& PushNil();
    CLuaArgumentsBuilder& PushBoolean(bool value);
    CLuaArgumentsBuilder& PushNumber(double value);
    CLuaArgumentsBuilder& PushString(const char *value);
    CLuaArgumentsBuilder& PushUserData(void *value);

private:
    CLuaArguments m_LuaArguments;
};
