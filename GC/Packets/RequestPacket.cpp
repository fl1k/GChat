#include "RequestPacket.h"

RequestPacket::RequestPacket(GNet::Packet packet)
{
	packet.Extract(this->message);
	packet.ResetOffset();
}

RequestPacket::RequestPacket(const std::string& message)
{
	this->message = message;
}

GNet::Packet RequestPacket::GeneratePacket() const
{
	GNet::Packet packet(PacketType::Request);
	packet.Insert(this->message);
	return packet;
}