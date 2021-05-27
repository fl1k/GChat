#pragma once
#include "Packet.h"
#include <queue>
#include <memory>
#include "PacketHandlerTask.h"

namespace GNet
{
	class PacketHandler : private std::queue<std::shared_ptr<Packet>>
	{
	public:
		PacketHandler();
		uint16_t currentPacketOffset;
		uint16_t currentPacketSize;
		PacketHandlerTask currentTask;
		bool HasPendingPackets();
		void Push(std::shared_ptr<Packet> p);
		std::shared_ptr<Packet> Front();
		void Pop();

	};
}