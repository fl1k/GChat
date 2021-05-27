#include "Client.h"
#include <GC/Packets/RequestPacket.h>
#include <GC/Packets/ChatMessagePacket.h>
#include <GC/Packets/ServerUpdatePacket.h>
#include "../SDK/SAMP.h"
#include "../Misc/debug.h"


bool Client::Initialize(GNet::IPEndpoint ipEndpoint)
{
	Client::connection = std::make_unique<GNet::TCPSocket>(GNet::IPVersion::IPv4);
	try { Client::connection->Connect(ipEndpoint); }
	catch (GNet::SocketException& exception)
	{
		SAMP::AddMessageToChat(exception.what().c_str(), EXCEPTION_COLOR);
		return false;
	}
	Client::connected = true;
	return true;
}

bool Client::Send(const char* request)
{
	try
	{
		RequestPacket requestPacket(request);
		try { Client::connection->Send(requestPacket.GeneratePacket()); }
		catch (GNet::SocketException& exception)
		{
			SAMP::AddMessageToChat(exception.what().c_str(), Client::EXCEPTION_COLOR);
			return false;
		}
	}
	catch (GNet::PacketException& exception)
	{
		SAMP::AddMessageToChat(exception.what().c_str(), Client::EXCEPTION_COLOR);
		return false;
	}
	return true;
}

bool Client::Shutdown()
{
	try
	{
		if (connection->IsValid() == true)
		{
			Client::connected = false;
			Client::connection->Close();
		}
	}
	catch (GNet::SocketException exception)
	{
		SAMP::AddMessageToChat(exception.what().c_str(), Client::EXCEPTION_COLOR);
		return false;
	}
	Client::connected = false;
	return true;
}

void Client::HandleReceive()
{
	while (true)
	{
		GNet::Packet packet;
		try
		{
			Client::connection->Recieve(packet);
			if (packet.GetPacketType() == PacketType::ChatMessage)
			{
				ChatMessagePacket chatMessagePacket(packet);
				std::string line = chatMessagePacket.username + ": " + chatMessagePacket.message;
				SAMP::AddMessageToChat(line.c_str(), Client::CHAT_COLOR);
			}
			else if (packet.GetPacketType() == PacketType::ServerUpdate)
			{
				ServerUpdatePacket serverUpdatePacket(packet);
				SAMP::AddMessageToChat(serverUpdatePacket.message.c_str(), serverUpdatePacket.color);
			}
			else
			{
				SAMP::AddMessageToChat("[ERROR] Bad packet received from server", Client::EXCEPTION_COLOR);
			}
		}
		catch (GNet::SocketException exception)
		{
			if (Client::connected == true)
				SAMP::AddMessageToChat(exception.what().c_str(), Client::EXCEPTION_COLOR);
			break;
		}
	}
}