#pragma once
#include <queue>
#include "../Packet/PacketHandler.h"
#include "../Socket/TCPSocket.h"
#include "../IPEndpoint/IPEndpoint.h"

namespace GNet
{
	class TCPConnection
	{
	private:
		IPEndpoint ipEndpoint;

	public:
		char buffer[Packet::MAX_PACKET_SIZE];
		PacketHandler incoming;
		PacketHandler outgoing;
		TCPSocket socket;
		TCPConnection() = delete;
		TCPConnection(TCPSocket socket, IPEndpoint ipEndpoint);
		std::string ToString() const;
		void Close();
	};
}