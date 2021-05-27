#include "ServerUpdatePacket.h"

ServerUpdatePacket::ServerUpdatePacket(GNet::Packet packet)
{
	packet.Extract(this->message);
	packet.Extract(this->color);
	packet.ResetOffset();
}

ServerUpdatePacket::ServerUpdatePacket(const std::string& message, uint32_t color)
{
	this->message = message;
	this->color = color;
}

GNet::Packet ServerUpdatePacket::GeneratePacket() const
{
	GNet::Packet packet(PacketType::ServerUpdate);
	packet.Insert(this->message);
	packet.Insert(this->color);
	return packet;
}
