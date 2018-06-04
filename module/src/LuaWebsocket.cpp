/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        LuaWebsocket.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "LuaWebsocket.h"
#include "WebsocketManager.h"
#include "ResourceManager.h"
#include "Module.h"

#include <cstring>

LuaWebsocket::LuaWebsocket(Resource resource)
	: m_WebsocketState(WebsocketState::Closed)
	, m_Resource(resource)
	, m_ResourceState(ResourceState::Running)
	, m_URI("")
{
	g_ResourceManager->AddWebsocket(resource, *this);
}

LuaWebsocket::LuaWebsocket(Resource resource, const std::string & uri)
	: LuaWebsocket(resource)
{
	m_URI = uri;
}

LuaWebsocket::~LuaWebsocket()
{
	if (m_Resource)
		g_ResourceManager->RemoveWebsocket(m_Resource, *this);
}

bool LuaWebsocket::Open()
{
	if (m_ResourceState != ResourceState::Running)
		return false;

	if (m_Websocket)
		return m_Websocket->Open(m_URI);

	m_Websocket = g_WebsocketManager->CreateWebsocket(m_URI);

	if (m_Websocket)
	{
		m_WebsocketState = WebsocketState::Connecting;

		m_Websocket->SetOpenHandler([this]() {
			m_WebsocketState = WebsocketState::Connected;

			if (m_OpenHandler.valid())
				HandleInvokeResult("open", m_OpenHandler(this));
		});

		m_Websocket->SetCloseHandler([this](int code, const std::string& message) {
			m_WebsocketState = WebsocketState::Closed;

			if (m_CloseHandler.valid())
				HandleInvokeResult("close", m_CloseHandler(this, code, message));
		});

		m_Websocket->SetFailHandler([this](int code, const std::string& message) {
			m_WebsocketState = WebsocketState::Closed;

			if (m_FailHandler.valid())
				HandleInvokeResult("fail", m_FailHandler(this, code, message));
		});

		m_Websocket->SetMessageHandler([this](int opcode, const std::string& payload) {
			if (m_MessageHandler.valid())
				HandleInvokeResult("message", m_MessageHandler(this, opcode, payload));
		});

		m_Websocket->Connect();
	}

	return m_Websocket != nullptr;
}

bool LuaWebsocket::Close(sol::optional<std::string> reason)
{
	if (m_ResourceState != ResourceState::Running)
		return false;

	if (!m_Websocket)
		return false;

	m_WebsocketState = WebsocketState::Closing;
	m_Websocket->Close(reason);

	return true;
}

bool LuaWebsocket::Send(const std::string& message, sol::optional<FrameOpcode> opcode)
{
	if (m_ResourceState == ResourceState::Stopped)
		return false;

	if (!m_Websocket)
		return false;

	return m_Websocket->Write(message, static_cast<WebsocketMeta::FrameOpcode>(opcode.value_or(FrameOpcode::Text)));
}

LuaWebsocket::WebsocketState LuaWebsocket::GetWebsocketState() const
{
	return m_WebsocketState;
}

LuaWebsocket::ResourceState LuaWebsocket::GetResourceState() const
{
	return m_ResourceState;
}

void LuaWebsocket::SetURI(const std::string& uri)
{
	m_URI = uri;
}

const std::string& LuaWebsocket::GetURI() const
{
	return m_URI;
}

bool LuaWebsocket::AddEventHandler(const char* eventName, sol::protected_function handler)
{
	if (m_ResourceState == ResourceState::Stopped)
		return false;

	if (!std::strcmp(eventName, "open"))
		m_OpenHandler = handler;
	else if (!std::strcmp(eventName, "close"))
		m_CloseHandler = handler;
	else if (!std::strcmp(eventName, "fail"))
		m_FailHandler = handler;
	else if (!std::strcmp(eventName, "message"))
		m_MessageHandler = handler;
	else if (!std::strcmp(eventName, "error"))
		m_ErrorHandler = handler;
	else
		return false;

	return true;
}

void LuaWebsocket::OnResourceStop()
{
	m_ResourceState = ResourceState::Stopping;

	if (m_Websocket)
	{
		Close(std::string{""});

		if (m_CloseHandler.valid() && m_WebsocketState != WebsocketState::Closed)
		{
			m_WebsocketState = WebsocketState::Closed;
			HandleInvokeResult("close", m_CloseHandler(this, -1, "Resource is stopping"));
		}
	}

	m_Resource = nullptr;
	m_ResourceState = ResourceState::Stopped;
	m_OpenHandler = sol::nil;
	m_CloseHandler = sol::nil;
	m_FailHandler = sol::nil;
	m_MessageHandler = sol::nil;
	m_ErrorHandler = sol::nil;
}

void LuaWebsocket::HandleInvokeResult(const char* eventName, sol::protected_function_result result)
{
	if (m_ErrorHandler.valid() && !result.valid())
	{
		sol::error err = result;
		m_ErrorHandler(this, eventName, err.what());
	}
}
