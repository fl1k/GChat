#include "PacketHandler.h"


namespace GNet
{
	PacketHandler::PacketHandler() :
		currentPacketOffset(0U),
		currentPacketSize(0U),
		currentTask(PacketHandlerTask::ProcessSize)
	{
	}

	void PacketHandler::Push(std::shared_ptr<Packet> p)
	{
		this->currentPacketSize = 0;
		this->currentPacketOffset = 0;
		this->currentTask = PacketHandlerTask::ProcessSize;
		this->push(std::move(p));
	}

	std::shared_ptr<Packet> PacketHandler::Front()
	{
		return this->front();
	}

	void PacketHandler::Pop()
	{
		this->currentPacketOffset = 0;
		this->currentTask = PacketHandlerTask::ProcessSize;
		this->pop();
	}

	bool PacketHandler::HasPendingPackets()
	{
		return this->size();
	}
}