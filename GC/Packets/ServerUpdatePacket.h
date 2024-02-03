#pragma once
#include <GNet/Core/Packet/Packet.h>
#include "PacketType.h"
#include <string>

class ServerUpdatePacket
{
public:
	std::string message;
	uint32_t color;
	ServerUpdatePacket() = delete;
	ServerUpdatePacket(GNet::Packet packet);
	ServerUpdatePacket(const std::string& message, uint32_t color);
	GNet::Packet GeneratePacket() const;
};