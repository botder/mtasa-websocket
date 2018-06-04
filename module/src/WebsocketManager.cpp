/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CConnectionManager.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "WebsocketManager.h"
#include "Module.h"

#include <algorithm>

using SSLContextPtr = std::shared_ptr<websocketpp::lib::asio::ssl::context>;
static SSLContextPtr ContextHandler(websocketpp::connection_hdl hdl);
static const std::string USER_AGENT = WSM_USER_AGENT;

WebsocketManager::WebsocketManager()
{
	m_Client.init_asio();
	m_Client.start_perpetual();
	m_Client.clear_access_channels(websocketpp::log::alevel::all);
	m_Client.clear_error_channels(websocketpp::log::elevel::all);
	m_Client.set_tls_init_handler(&ContextHandler);
	m_Client.set_user_agent(USER_AGENT);

	m_Thread = std::thread{ &Client::run, &m_Client };
}

WebsocketManager::~WebsocketManager()
{
	m_Client.stop_perpetual();

	for (ConnectionMetaMap::reference pair : m_ConnectionMeta)
	{
		const ConnectionPtr& con = pair.first;

		if (con->get_state() != websocketpp::session::state::open)
			continue;

		std::error_code ec;
		con->close(websocketpp::close::status::going_away, "", ec);

		pair.second->m_Manager.reset();
		pair.second->m_HDL.reset();
		pair.second->onClose();
	}

	ConnectionMetaMap{}.swap(m_ConnectionMeta);
	m_Thread.join();
}

WebsocketManager::MetaPtr WebsocketManager::CreateWebsocket(const std::string& uri)
{
	std::error_code ec;
	ConnectionPtr con = m_Client.get_connection(uri, ec);

	if (ec)
		return nullptr;

	MetaPtr meta = std::make_shared<WebsocketMeta>();
	meta->m_Manager = shared_from_this();
	meta->m_HDL = con->get_handle();
	m_ConnectionMeta.emplace(std::make_pair(con, meta));

	con->set_open_handler(std::bind(&WebsocketMeta::onOpen, meta));
	con->set_close_handler(std::bind(&WebsocketMeta::onClose, meta));
	con->set_fail_handler(std::bind(&WebsocketMeta::onFail, meta));
	con->set_message_handler(std::bind(&WebsocketMeta::onMessage, meta, std::placeholders::_2));

	return meta;
}

WebsocketManager::ConnectionPtr WebsocketManager::GetConnection(websocketpp::connection_hdl hdl)
{
	return m_Client.get_con_from_hdl(hdl);
}

void WebsocketManager::Connect(ConnectionPtr con)
{
	m_Client.connect(con);
}

SSLContextPtr ContextHandler(websocketpp::connection_hdl hdl)
{
	namespace asio = websocketpp::lib::asio;
	SSLContextPtr ctx = std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23_client);

	boost::system::error_code ec;

	ctx->set_options(asio::ssl::context::default_workarounds |
		asio::ssl::context::no_sslv2 |
		asio::ssl::context::no_sslv3 |
		asio::ssl::context::single_dh_use, ec);

	return ctx;
}
