/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Resource.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

struct lua_State;

class Resource
{
public:
    Resource(lua_State* pLuaVM);
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource(Resource&&) = default;
    Resource& operator=(Resource&&) = default;
    ~Resource() = default;

    void OnResourceStart();
    void OnResourceStop();

    bool IsRunning() const { return m_pLuaVM != nullptr; }

private:
    lua_State* m_pLuaVM;
};
