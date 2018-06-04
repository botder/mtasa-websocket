/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CConnectionManager.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once
#pragma warning(disable: 4267)

#include "WebsocketMeta.h"

#include <map>
#include <string>
#include <thread>
#include <memory>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

class WebsocketManager : public std::enable_shared_from_this<WebsocketManager>
{
public:
	using Client = websocketpp::client<websocketpp::config::asio_tls_client>;
	using ConnectionPtr = Client::connection_ptr;
	using MetaPtr = std::shared_ptr<WebsocketMeta>;
	using ConnectionMetaMap = std::map<ConnectionPtr, MetaPtr>;

public:
	WebsocketManager();
	~WebsocketManager();

public:
	MetaPtr CreateWebsocket(const std::string& uri);
	ConnectionPtr GetConnection(websocketpp::connection_hdl hdl);
	void Connect(ConnectionPtr con);
	
private:
	ConnectionMetaMap m_ConnectionMeta;
	std::thread m_Thread;
	Client m_Client;
};
