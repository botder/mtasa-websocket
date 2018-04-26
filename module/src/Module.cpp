/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Module.cpp
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "Module.h"
#include "luaimports.h"
#include <signal.h>

ILuaModuleManager10 *pModuleManager = NULL;
bool ms_bInitWorked = false;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule ( ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion )
{
	pModuleManager = pManager;

	// Set the module info
	strncpy ( szModuleName, MODULE_NAME, MAX_INFO_LENGTH );
	strncpy ( szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH );
	(*fVersion) = MODULE_VERSION;

    if ( !ImportLua() )
    {
        return false;
    }

#ifdef WIN32
    WSADATA wsaData;
    if ( WSAStartup ( MAKEWORD ( 2, 2 ), &wsaData ) != 0 )
    {
        pModuleManager->ErrorPrintf("[Sockets] Can't start Winsock, aborting...");
        return false;
    }
#else
    // Avoid process termination on broken pipe
    signal(SIGPIPE, SIG_IGN);
#endif

    ms_bInitWorked = true;
	return true;
}


MTAEXPORT void RegisterFunctions ( lua_State * luaVM )
{
    if ( !ms_bInitWorked )
        return;

	if ( pModuleManager && luaVM )
	{
        // Register functions
        pModuleManager->RegisterFunction ( luaVM, "sockOpen",  CFunctions::sockOpen );
        pModuleManager->RegisterFunction ( luaVM, "sockWrite", CFunctions::sockWrite );
        pModuleManager->RegisterFunction ( luaVM, "sockClose", CFunctions::sockClose );

        // Add events
        CFunctions::AddEvent ( luaVM, "onSockOpened" );
        CFunctions::AddEvent ( luaVM, "onSockData" );
        CFunctions::AddEvent ( luaVM, "onSockClosed" );
	}
}


MTAEXPORT bool DoPulse ( void )
{
    CSocketManager::DoPulse();
	return true;
}


MTAEXPORT void ResourceStopped ( lua_State * luaVM )
{
    CSocketManager::ResourceStopped( luaVM );
}


MTAEXPORT bool ShutdownModule ( void )
{
    // Stop all sockets before shutting down the module
    CSocketManager::HandleStop ( );

#ifdef WIN32
    // Cleanup Winsock stuff
    WSACleanup ( );
#endif

	return true;
}
