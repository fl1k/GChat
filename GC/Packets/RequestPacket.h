#pragma once
#include <GNet/Core/Packet/Packet.h>
#include "PacketType.h"
#include <string>

class RequestPacket
{
public:
	std::string message;
	RequestPacket() = delete;
	RequestPacket(GNet::Packet packet);
	RequestPacket(const std::string& message);
	GNet::Packet GeneratePacket() const;
};