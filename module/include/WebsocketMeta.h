/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        WebsocketMeta.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <string>
#include <memory>
#include <functional>
#include <sol.hpp>

class WebsocketManager;

namespace websocketpp
{
	using connection_hdl = std::weak_ptr<void>;

	namespace message_buffer
	{
		namespace alloc
		{
			template <typename message>
			class con_msg_manager;
		}

		template <template<class> class con_msg_manager>
		class message;
	}

	namespace frame
	{
		namespace opcode
		{
			enum value;
		}
	}
}

class WebsocketMeta
{
	friend class WebsocketManager;
	using MessagePtr = std::shared_ptr<websocketpp::message_buffer::message<websocketpp::message_buffer::alloc::con_msg_manager>>;

public:
	using FrameOpcode = websocketpp::frame::opcode::value;

public:
	using OpenHandler    = std::function<void()>;
	using CloseHandler   = std::function<void(int, const std::string&)>;
	using FailHandler    = std::function<void(int, const std::string&)>;
	using MessageHandler = std::function<void(int, const std::string&)>;

public:
	bool Open(sol::optional<std::string> uri);
	bool Connect();
	bool Write(const std::string& payload, FrameOpcode opcode);
	bool Close(sol::optional<std::string> reason);

public:
	void SetOpenHandler(OpenHandler handler);
	void SetCloseHandler(CloseHandler handler);
	void SetFailHandler(FailHandler handler);
	void SetMessageHandler(MessageHandler handler);

private:
	void onOpen();
	void onClose();
	void onFail();
	void onMessage(MessagePtr message);

private:
	OpenHandler    m_OpenHandler;
	CloseHandler   m_CloseHandler;
	FailHandler    m_FailHandler;
	MessageHandler m_MessageHandler;

private:
	std::weak_ptr<WebsocketManager> m_Manager;
	websocketpp::connection_hdl     m_HDL;
};
