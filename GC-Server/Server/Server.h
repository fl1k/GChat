#pragma once
#include "Client/Client.h"
#include "DisconnectReason.h"
#include "Database/Database.h"

#include "Command/Command.h"
#include <GNet/Core/Packet/Packet.h>
#include <GC/Packets/RequestPacket.h>
#include <GC/Packets/ServerUpdatePacket.h>
#include <GC/Packets/ChatMessagePacket.h>
#include <GC/Packets/PacketType.h>
#include "../Misc/Logger.h"

#include <poll.h>
#include <unistd.h>
#include <algorithm>
#include <cctype>
#include <string>


#ifdef _WIN32
typedef WSAPOLLFD pollfd;
#define poll WSAPoll
#endif

class Server
{
private:
	static const size_t DEFAULT_MAX_CLIENTS = 128U;
	static const size_t COMMAND_ERROR_COLOR = 0xB80000;
	static const size_t COMMAND_SUCCESS_COLOR = 0x008000;
	static const size_t CONNECTIONUPDATE_COLOR = 0x808080;

	size_t maxClients;
	GNet::IPEndpoint ipEndpoint;
	GNet::TCPSocket listeningSocket;
	Database database;
	std::vector<Client> clients;
	std::vector<pollfd> masterFds;
	void TryAcceptNewConnection();
	bool ProcessPacket(size_t clientIndex, std::shared_ptr<GNet::Packet> packet);
	void CloseConnection(size_t clientIndex, DisconnectReason reason);
	void SendAll(const GNet::Packet& packet);
	bool IsLoggedIn(const std::string& lowerUsername);

	void OnLogin(size_t clientIndex);
	void OnLogout(size_t clientIndex);
	void OnDisconnect(size_t clientIndex, DisconnectReason reason);

	uint32_t GetLoggedInCount() const;
public:
	Server() = delete;
	Server(uint16_t port, size_t maxClients = DEFAULT_MAX_CLIENTS);
	bool Initialize();
	bool Present();
	void Shutdown();
};