/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Common.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include "lua.hpp"

#define MAX_ARGUMENTS 10

struct FunctionArguments
{
	lua_State* luaVM;
	unsigned char nArguments;
	unsigned char Type[10];
	void*	Arguments[10];
};

namespace FunctionArgumentType
{
	enum
	{
		TYPE_NUMBER = 1,
		TYPE_STRING = 2,
		TYPE_LIGHTUSERDATA = 3,
		TYPE_BOOLEAN = 4,
		TYPE_NIL = 5,
		TYPE_TABLE = 6
	};
}
