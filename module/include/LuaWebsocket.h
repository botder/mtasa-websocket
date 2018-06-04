/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        LuaWebsocket.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <memory>
#include <string>
#include <sol.hpp>

class WebsocketMeta;

class LuaWebsocket
{
public:
	using Resource = struct lua_State *;

	enum class ResourceState
	{
		Running  = 0,
		Stopping = 1,
		Stopped  = 2,
	};

	enum class WebsocketState
	{
		Closed     = 0,
		Closing    = 1,
		Connecting = 2,
		Connected  = 3,
	};

	enum class FrameOpcode
	{
		Continuation = 0,
		Text	     = 1,
		Binary       = 2,
	};

public:
	LuaWebsocket(Resource resource);
	LuaWebsocket(Resource resource, const std::string& uri);
	LuaWebsocket(const LuaWebsocket&) = delete;
	LuaWebsocket(LuaWebsocket&&) = delete;
	LuaWebsocket& operator=(const LuaWebsocket&) = delete;
	LuaWebsocket& operator=(LuaWebsocket&&) = delete;
	~LuaWebsocket();

public:
	bool               Open();
	bool               Close(sol::optional<std::string> reason);
	bool               Send(const std::string& message, sol::optional<FrameOpcode> opcode);
	void               SetURI(const std::string& uri);
	const std::string& GetURI() const;
	bool               AddEventHandler(const char* eventName, sol::protected_function handler);
	WebsocketState     GetWebsocketState() const;
	ResourceState      GetResourceState() const;

public:
	void OnResourceStop();

private:
	void HandleInvokeResult(const char* eventName, sol::protected_function_result result);

private:
	std::shared_ptr<WebsocketMeta> m_Websocket;
	WebsocketState                 m_WebsocketState;
	Resource					   m_Resource;
	ResourceState                  m_ResourceState;
	std::string					   m_URI;
	sol::protected_function        m_OpenHandler;
	sol::protected_function        m_CloseHandler;
	sol::protected_function        m_FailHandler;
	sol::protected_function        m_MessageHandler;
	sol::safe_function             m_ErrorHandler;
};
