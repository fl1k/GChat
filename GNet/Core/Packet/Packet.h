#pragma once
#include <vector>
#include <string>
#include "PacketException.h"

namespace GNet
{
	class Packet : private std::vector<char>
	{
	private:
		uint32_t readOffset;
		void Insert(const void* buffer, uint32_t length);
		void Extract(void* buffer, uint32_t length);
	public:
		static const uint32_t MAX_PACKET_SIZE = 1024;
		static const uint32_t PACKET_START_SIZE = sizeof(uint16_t);

		Packet();
		Packet(uint16_t packetType);
		void Resize(uint32_t size);
		void Insert(const uint32_t value);
		void Insert(const std::string& value);
		void Extract(uint32_t& value);
		void Extract(std::string& value);
		void ResetOffset();
		void Clear();
		void* Buffer();
		const void* Data() const;
		size_t GetSize() const;
		uint16_t GetPacketType() const;
	};
}