/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        SocketErrors.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

// General
#define ERR_INVALID_SOCKET  -1
#define ERR_SEND_FAILURE    -1
#define ERR_CONNECT_FAILURE -1
#define ERR_CONNECT_SUCCESS 0
#define ERR_NO_ERROR        0


#ifdef WIN32
    // Win32 specific
    #define ERR_WOULD_BLOCK             WSAEWOULDBLOCK
    #define ERR_CONNECT_IN_PROGRESS     WSAEWOULDBLOCK
    #define ERR_NOT_CONNECTED           WSAENOTCONN
#else
    // Linux specific
    #define ERR_WOULD_BLOCK             EWOULDBLOCK
    #define ERR_CONNECT_IN_PROGRESS     EINPROGRESS
    #define ERR_NOT_CONNECTED           ENOTCONN
#endif
