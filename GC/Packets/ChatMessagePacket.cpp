#include "ChatMessagePacket.h"

ChatMessagePacket::ChatMessagePacket(GNet::Packet packet)
{
	packet.Extract(this->username);
	packet.Extract(this->message);
	packet.ResetOffset();
}

ChatMessagePacket::ChatMessagePacket(const std::string& username, const std::string& message)
{
	this->username = username;
	this->message = message;
}

GNet::Packet ChatMessagePacket::GeneratePacket() const
{
	GNet::Packet packet(PacketType::ChatMessage);
	packet.Insert(this->username);
	packet.Insert(this->message);
	return packet;
}
