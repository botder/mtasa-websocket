/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        WebsocketMeta.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "WebsocketMeta.h"
#include "WebsocketManager.h"

using ConnectionPtr = WebsocketManager::ConnectionPtr;
using WebsocketManagerPtr = std::shared_ptr<WebsocketManager>;

static ConnectionPtr GetConnectionFromManager(std::weak_ptr<WebsocketManager> m, websocketpp::connection_hdl hdl)
{
	if (m.expired() || hdl.expired())
		return nullptr;

	WebsocketManagerPtr manager = m.lock();

	if (!manager)
		return nullptr;

	return manager->GetConnection(hdl);
}

bool WebsocketMeta::Open(sol::optional<std::string> uri)
{
	if (m_Manager.expired())
		return false;

	WebsocketManagerPtr manager = m_Manager.lock();

	if (!manager)
		return false;

	if (!m_HDL.expired())
	{
		ConnectionPtr con = manager->GetConnection(m_HDL);

		if (con && con->get_state() == websocketpp::session::state::open)
		{
			std::error_code ec;
			con->close(websocketpp::close::status::abnormal_close, "", ec);
		}

		if (uri)
		{
			websocketpp::uri_ptr location = websocketpp::lib::make_shared<websocketpp::uri>(uri.value());

			if (!location->get_valid())
				return false;

			con->set_uri(location);
		}

		manager->Connect(con);
	}
	else
	{
		// Create a new connection
	}

	return true;
}

bool WebsocketMeta::Connect()
{
	if (m_Manager.expired())
		return false;

	WebsocketManagerPtr manager = m_Manager.lock();

	if (!manager)
		return false;

	ConnectionPtr con = GetConnectionFromManager(m_Manager, m_HDL);

	if (!con)
		return false;

	manager->Connect(con);
	return true;
}

bool WebsocketMeta::Write(const std::string& payload, FrameOpcode opcode)
{
	ConnectionPtr con = GetConnectionFromManager(m_Manager, m_HDL);

	if (!con)
		return false;

	con->send(payload, opcode);
	return true;
}

bool WebsocketMeta::Close(sol::optional<std::string> reason)
{
	ConnectionPtr con = GetConnectionFromManager(m_Manager, m_HDL);

	if (!con)
		return false;

	std::error_code ec;
	con->close(websocketpp::close::status::going_away, reason.value_or(""), ec);
	
	return !ec;
}

void WebsocketMeta::SetOpenHandler(OpenHandler handler)
{
	m_OpenHandler = handler;
}

void WebsocketMeta::SetCloseHandler(CloseHandler handler)
{
	m_CloseHandler = handler;
}

void WebsocketMeta::SetFailHandler(FailHandler handler)
{
	m_FailHandler = handler;
}

void WebsocketMeta::SetMessageHandler(MessageHandler handler)
{
	m_MessageHandler = handler;
}

void WebsocketMeta::onOpen()
{
	if (m_OpenHandler)
		m_OpenHandler();
}

void WebsocketMeta::onClose()
{
	if (!m_CloseHandler)
		return;

	ConnectionPtr con = GetConnectionFromManager(m_Manager, m_HDL);

	if (!con)
		return m_CloseHandler(-1, "");

	m_CloseHandler(con->get_remote_close_code(), con->get_remote_close_reason());
}

void WebsocketMeta::onFail()
{
	if (!m_FailHandler)
		return;

	ConnectionPtr con = GetConnectionFromManager(m_Manager, m_HDL);

	if (!con)
		return m_FailHandler(-1, "");

	std::error_code ec = con->get_ec();
	m_FailHandler(ec.value(), ec.message());
}

void WebsocketMeta::onMessage(MessagePtr message)
{
	if (m_MessageHandler)
	{
		int opcode = message->get_opcode();
		const std::string& payload = message->get_payload();

		if (opcode == websocketpp::frame::opcode::text)
			m_MessageHandler(opcode, payload);
		else
			m_MessageHandler(opcode, websocketpp::utility::to_hex(payload));
	}
}
