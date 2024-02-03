#pragma once
#include <GNet/Core/Packet/Packet.h>
#include "PacketType.h"
#include <string>

class ChatMessagePacket
{
public:
	std::string username;
	std::string message;
	ChatMessagePacket() = delete;
	ChatMessagePacket(GNet::Packet packet);
	ChatMessagePacket(const std::string& username, const std::string& message);
	GNet::Packet GeneratePacket() const;
};