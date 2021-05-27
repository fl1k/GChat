#include "Packet.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#else
#include <sys/>
#endif

namespace GNet
{
	void Packet::Insert(const void* buffer, uint32_t length)
	{
		if ((this->size() + length) > MAX_PACKET_SIZE)
			throw PacketException("GNet::Packet::Insert - Attempting to add to packet over max packet size.");
		this->insert(this->end(), reinterpret_cast<const char*>(buffer), reinterpret_cast<const char*>(buffer) + length);
	}

	void Packet::Extract(void* buffer, uint32_t length)
	{
		if (this->readOffset + length > this->size())
			throw PacketException("GNet::Packet::Extract - Attempting to read packet over its size.");
		memcpy(buffer, &(this->data()[this->readOffset]), length);
		this->readOffset += length;
	}

	Packet::Packet() : Packet(0U)
	{
	}

	Packet::Packet(uint16_t packetType) :
		readOffset(PACKET_START_SIZE)
	{
		packetType = htons(packetType);
		this->resize(PACKET_START_SIZE);
		*reinterpret_cast<uint16_t*>(this->data()) = packetType;
	}

	void Packet::Resize(const uint32_t size)
	{
		if (this->size() + size > MAX_PACKET_SIZE)
			throw PacketException("GNet::Packet::Resize - Attempting to resize packet over MAX_PACKET_SIZE.");
		this->resize(size);
	}

	void Packet::Insert(const uint32_t value)
	{
		uint32_t networkValue = htonl(value);;
		this->Insert(&networkValue, sizeof(uint32_t));
	}

	void Packet::Insert(const std::string& value)
	{
		this->Insert(value.size());
		this->Insert(value.data(), value.size());
	}

	void Packet::Extract(uint32_t& value)
	{
		this->Extract(&value, sizeof(uint32_t));
		value = ntohl(value);
	}

	void Packet::Extract(std::string& value)
	{
		value.clear();
		uint32_t stringSize;
		this->Extract(stringSize);
		if (stringSize > MAX_PACKET_SIZE)
			throw PacketException("GNet::Packet::Extract - Attempted extracting string size over MAX_PACKET_SIZE.");
		value.resize(stringSize);
		this->Extract(value.data(), stringSize);
	}

	void Packet::ResetOffset()
	{
		this->readOffset = PACKET_START_SIZE;
	}

	void Packet::Clear()
	{
		this->readOffset = PACKET_START_SIZE;
		this->clear();
		this->resize(PACKET_START_SIZE);
	}

	void* Packet::Buffer()
	{
		return this->data();
	}

	const void* Packet::Data() const
	{
		return this->data();
	}

	size_t Packet::GetSize() const
	{
		return this->size();
	}

	uint16_t Packet::GetPacketType() const
	{
		uint16_t packetType = *reinterpret_cast<const uint16_t*>(this->data());
		return ntohs(packetType);
	}
}